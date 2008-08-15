#include "map.h"

#include "camera.h"
#include "imageLibrary.h"
#include "world.h"
#include "imageEntity.h"

namespace pammo
{
    
Map::Map()
    : View()
{
    memset(mBuckets, 0, sizeof(mBuckets));
}
    
Map::~Map()
{
}

void Map::setBackdrop(RawImage* raw)
{
    mSize = raw->mSize;
    mBucketSize.x = mSize.x / kMapBucketCount;
    mBucketSize.y = mSize.y / kMapBucketCount;
    
    uint32_t texSize = 128;
    for(uint32_t x=0; x < mSize.x/texSize; ++x)
    {
        for(uint32_t y=0; y < mSize.y/texSize; ++y)
        {
            Image* img = makeSubImage(raw, Vector2(x*texSize, y*texSize), Vector2(texSize, texSize));
            ImageEntity* entity = new ImageEntity(img);
            entity->mCenter = Vector2(x*texSize + texSize/2, y*texSize + texSize/2);
            addProp(entity);
        }
    }
}

void Map::addProp(ImageEntity* entity)
{
    mEntities.push_back(entity);
    
    Vector2 ul = entity->mCenter - entity->mSize/2;
    Vector2 br = entity->mCenter + entity->mSize/2;
    
    ul.x = roundf(ul.x); ul.y = roundf(ul.y);
    br.x = roundf(br.x); br.y = roundf(br.y);
    
    // If the prop is completely off the map, return;
    if(br.x <= 0 || br.y <= 0 || ul.x >= mSize.x || ul.y >= mSize.y)
        return;
    
    int16_t startX = floor(ul.x/mBucketSize.x);
    int16_t startY = floor(ul.y/mBucketSize.y);
    int16_t endX = floor(br.x/mBucketSize.x);
    int16_t endY = floor(br.y/mBucketSize.y);
    
    if(startX < 0) startX = 0;
    if(startY < 0) startY = 0;
    if(endX >= kMapBucketCount) endX = kMapBucketCount-1;
    if(endY >= kMapBucketCount) endY = kMapBucketCount-1;

    for(uint16_t x=startX; x <= endX; ++x)
    {
        for(uint16_t y=startY; y <= endY; ++y)
        {
            addSubProp(entity->getImage(), ul, br, x, y);
        }
    }
}

void Map::addSubProp(Image* image, Vector2 ul, Vector2 br, uint16_t x, uint16_t y)
{
    Prop* prop = new Prop();
    prop->mImage = image;
    
    prop->mTexCoords[0] = Vector2(0, 0);
    prop->mTexCoords[1] = Vector2(1, 0);
    prop->mTexCoords[2] = Vector2(0, 1);
    prop->mTexCoords[3] = Vector2(1, 1);
    
    prop->mVertecies[0] = Vector2(ul.x, ul.y);
    prop->mVertecies[1] = Vector2(br.x, ul.y);
    prop->mVertecies[2] = Vector2(ul.x, br.y);
    prop->mVertecies[3] = Vector2(br.x, br.y);
    
    float clamp;
    
    // Clamp left.
    clamp = x*mBucketSize.x;
    if(ul.x < clamp)
    {
        float over = (clamp - ul.x) / (br.x - ul.x);
        prop->mTexCoords[0].x = over;
        prop->mTexCoords[2].x = over;
        prop->mVertecies[0].x = clamp;
        prop->mVertecies[2].x = clamp;
    }
    
    // Clamp top.
    clamp = y*mBucketSize.y;
    if(ul.y < clamp)
    {
        float over = (clamp - ul.y) / (br.y - ul.y);
        prop->mTexCoords[0].y = over;
        prop->mTexCoords[1].y = over;
        prop->mVertecies[0].y = clamp;
        prop->mVertecies[1].y = clamp;
    }
    
    // Clamp right.
    clamp = (x+1)*mBucketSize.x;
    if(br.x > clamp)
    {
        float over = 1 - (br.x - clamp) / (br.x - ul.x);
        prop->mTexCoords[1].x = over;
        prop->mTexCoords[3].x = over;
        prop->mVertecies[1].x = clamp;
        prop->mVertecies[3].x = clamp;
    }
    
    // Clamp bottom.
    clamp = (y+1)*mBucketSize.y;
    if(br.y > clamp)
    {
        float over = 1 - (br.y - clamp) / (br.y - ul.y);
        prop->mTexCoords[2].y = over;
        prop->mTexCoords[3].y = over;
        prop->mVertecies[2].y = clamp;
        prop->mVertecies[3].y = clamp;
    }
    
    prop->mNext = 0;
    
    Prop* cur = mBuckets[x][y];
    if(cur)
    {
       while(cur->mNext) cur = cur->mNext;
       cur->mNext = prop;
    }
    else
    {
        mBuckets[x][y] = prop;
    }
}

uint32_t Map::getDrawPriority() const
{
    return kMapPriority;
}

// Sets up an array of values for the texture coordinates.
const GLfloat spriteTexcoords[] = {
0.0, 0.0,
1.0, 0.0,
0.0, 1.0,
1.0, 1.0,
};
    
void Map::draw()
{
    Camera* camera = gWorld->getCamera();
    camera->set();
    
    // Calculate lower-right and upper-left corners of camera view.
    assert(camera->mRotation == 0);
    Vector2 ul = (camera->mCenter - camera->mSize/2) / mBucketSize;
    if(ul.x < 0) ul.x = 0;
    if(ul.x > kMapBucketCount) ul.x = kMapBucketCount;
    if(ul.y < 0) ul.y = 0;
    if(ul.y > kMapBucketCount) ul.y = kMapBucketCount;
    Vector2 lr = (camera->mCenter + camera->mSize/2) / mBucketSize;
    if(lr.x < 0) lr.x = 0;
    if(lr.x > kMapBucketCount) lr.x = kMapBucketCount;
    if(lr.y < 0) lr.y = 0;
    if(lr.y > kMapBucketCount) lr.y = kMapBucketCount;
    
    uint16_t startX = floorf(ul.x);
    uint16_t startY = floorf(ul.y);
    uint16_t endX = ceilf(lr.x);
    uint16_t endY = ceilf(lr.y);
    
    glLoadIdentity();
    for(uint16_t y=startY; y < endY; ++y)
    {
        for(uint16_t x=startX; x < endX; ++x)
        {
            Prop* prop = mBuckets[x][y];
            while(prop)
            {
                glTexCoordPointer(2, GL_FLOAT, 0, (float*)prop->mTexCoords);
                glVertexPointer(2, GL_FLOAT, 0, (float*)prop->mVertecies);
                glBindTexture(GL_TEXTURE_2D, prop->mImage->mTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                prop = prop->mNext;
            }
        }
    }
    
    camera->unset();
}

Vector2 Map::getSize() const
{
    return mSize;
}

}