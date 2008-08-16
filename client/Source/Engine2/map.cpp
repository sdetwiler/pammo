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
    mBuckets = 0;
}
    
Map::~Map()
{
}

void Map::setBackdrop(RawImage* raw)
{
    mSizeX = raw->mSize.x;
    mSizeY = raw->mSize.y;
    mBucketSizeX = 128;
    mBucketSizeY = 128;
    mBucketCountX = ceilf(raw->mSize.x/mBucketSizeX);
    mBucketCountY = ceilf(raw->mSize.y/mBucketSizeY);
    
    mBuckets = new Prop*[mBucketSizeX * mBucketSizeY];
    
    uint32_t previousYCoord = 0;
    for(uint32_t y=0; y < mBucketCountY; ++y)
    {
        uint32_t currentYCoord = previousYCoord + mBucketSizeY;
        uint32_t previousXCoord = 0;
        for(uint32_t x=0; x < mBucketCountX; ++x)
        {
            uint32_t currentXCoord = previousXCoord + mBucketSizeX;
            
            Prop* prop = new Prop();
            prop->mImage = makeSubImage(raw, Vector2(previousXCoord, previousYCoord), Vector2(mBucketSizeX, mBucketSizeY));
            prop->mNext = 0;
            mBuckets[x + y*mBucketCountX] = prop;
    
            prop->mTexCoords[0] = Vector2(0, 0);
            prop->mTexCoords[1] = Vector2(1, 0);
            prop->mTexCoords[2] = Vector2(0, 1);
            prop->mTexCoords[3] = Vector2(1, 1);
    
            prop->mVertecies[0] = Vector2(previousXCoord, previousYCoord);
            prop->mVertecies[1] = Vector2(currentXCoord,  previousYCoord);
            prop->mVertecies[2] = Vector2(previousXCoord, currentYCoord);
            prop->mVertecies[3] = Vector2(currentXCoord,  currentYCoord);
            
            previousXCoord = currentXCoord;
        }
        previousYCoord = currentYCoord;
    }
}

void Map::addProp(ImageEntity* entity)
{
#if 0
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
    #endif
}

void Map::addSubProp(Image* image, Vector2 ul, Vector2 br, uint16_t x, uint16_t y)
{
    #if 0
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
    #endif
}

uint32_t Map::getDrawPriority() const
{
    return kMapPriority;
}
    
void Map::draw()
{
    Camera* camera = gWorld->getCamera();
    camera->set();
    
    // Calculate lower-right and upper-left corners of camera view.
    assert(camera->mRotation == 0);
    Vector2 ul = camera->mCenter - camera->mSize/2;
    Vector2 lr = camera->mCenter + camera->mSize/2;
    
    if(ul.x < 0) ul.x = 0;
    if(ul.x > mSizeX) ul.x = mSizeX;
    if(ul.y < 0) ul.y = 0;
    if(ul.y > mSizeY) ul.y = mSizeY;
    
    if(lr.x < 0) lr.x = 0;
    if(lr.x > mSizeX) lr.x = mSizeX;
    if(lr.y < 0) lr.y = 0;
    if(lr.y > mSizeY) lr.y = mSizeY;
    
    uint16_t startX = floorf(ul.x / mBucketSizeX);
    uint16_t startY = floorf(ul.y / mBucketSizeY);
    uint16_t endX = ceilf(lr.x / mBucketSizeX);
    uint16_t endY = ceilf(lr.y / mBucketSizeY);
    
    glLoadIdentity();
    for(uint16_t y=startY; y < endY; ++y)
    {
        for(uint16_t x=startX; x < endX; ++x)
        {
            Prop* prop = mBuckets[x + y*mBucketCountX];
            //while(prop)
            {
                glTexCoordPointer(2, GL_FLOAT, 0, (float*)prop->mTexCoords);
                glVertexPointer(2, GL_FLOAT, 0, (float*)prop->mVertecies);
                glBindTexture(GL_TEXTURE_2D, prop->mImage->mTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                //prop = prop->mNext;
            }
        }
    }
    
    camera->unset();
}

Vector2 Map::getSize() const
{
    return Vector2(mSizeX, mSizeY);
}

}