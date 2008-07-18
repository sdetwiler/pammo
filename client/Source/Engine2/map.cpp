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
    mNumMaterials = 0;
    mMaterials = 0;
    mSizeMaterial = 0;
    mSizeX = 0;
    mSizeY = 0;
    mTiles = NULL;
}
    
Map::~Map()
{
    if(mMaterials)
    {
        for(uint32_t i=0; i < mNumMaterials; ++i)
        {
            gImageLibrary->unreference(mMaterials[i]);
        }
        delete[] mMaterials;
    }
    
    if(mTiles)
        delete[] mTiles;
}

void Map::setNumMaterials(uint16_t numMaterials, uint16_t sizeMaterial)
{
    // Verify that this function has not yet been called.
    assert(mNumMaterials == 0);
    assert(mSizeMaterial == 0);
    assert(mMaterials == NULL);

    mNumMaterials = numMaterials;
    mSizeMaterial = sizeMaterial;
    
    mMaterials = new Image*[numMaterials];
    memset(mMaterials, 0, sizeof(void*)*numMaterials);
}
    
void Map::setMaterial(uint16_t index, char const* materialPath)
{
    // Verify that this slot hasn't been set yet.
    assert(mMaterials[index] == 0);
    
    Image* img = gImageLibrary->reference(materialPath);
    assert(img != 0);
    mMaterials[index] = img;
}
    
void Map::setNumTiles(uint16_t sizeX, uint16_t sizeY)
{
    assert(mSizeX == 0);
    assert(mSizeY == 0);
    assert(mTiles == NULL);
    
    mSizeX = sizeX;
    mSizeY = sizeY;
    
    mTiles = new uint16_t[sizeX*sizeY];
    memset(mTiles, 0, sizeof(uint16_t)*sizeX*sizeY);
    
    mProps = new Prop*[sizeX*sizeY];
    memset(mProps, 0, sizeof(Prop*)*sizeX*sizeY);
}
    
void Map::setTile(uint16_t x, uint16_t y, uint16_t index)
{
    assert(x < mSizeX);
    assert(y < mSizeY);
    assert(index < mNumMaterials);
    
    mTiles[x + y*mSizeY] = index;
}

void Map::addProp(ImageEntity* entity)
{
    mEntities.push_back(entity);
    
    Vector2 ul = entity->mCenter - entity->mSize/2;
    Vector2 br = entity->mCenter + entity->mSize/2;
    
    // If the prop is completely off the map, return;
    if(br.x <= 0 || br.y <= 0 || ul.x >= mSizeX*mSizeMaterial || ul.y >= mSizeY*mSizeMaterial)
        return;
    
    int16_t startX = floor(ul.x/mSizeMaterial);
    int16_t startY = floor(ul.y/mSizeMaterial);
    int16_t endX = floor(br.x/mSizeMaterial);
    int16_t endY = floor(br.y/mSizeMaterial);
    
    if(startX < 0) startX = 0;
    if(startY < 0) startY = 0;
    if(endX >= mSizeX) endX = mSizeX-1;
    if(endY >= mSizeY) endY = mSizeY-1;

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
    clamp = x*mSizeMaterial;
    if(ul.x < clamp)
    {
        float over = (clamp - ul.x) / (br.x - ul.x);
        prop->mTexCoords[0].x = over;
        prop->mTexCoords[2].x = over;
        prop->mVertecies[0].x = clamp;
        prop->mVertecies[2].x = clamp;
    }
    
    // Clamp top.
    clamp = y*mSizeMaterial;
    if(ul.y < clamp)
    {
        float over = (clamp - ul.y) / (br.y - ul.y);
        prop->mTexCoords[0].y = over;
        prop->mTexCoords[1].y = over;
        prop->mVertecies[0].y = clamp;
        prop->mVertecies[1].y = clamp;
    }
    
    // Clamp right.
    clamp = (x+1)*mSizeMaterial;
    if(br.x > clamp)
    {
        float over = 1 - (br.x - clamp) / (br.x - ul.x);
        prop->mTexCoords[1].x = over;
        prop->mTexCoords[3].x = over;
        prop->mVertecies[1].x = clamp;
        prop->mVertecies[3].x = clamp;
    }
    
    // Clamp bottom.
    clamp = (y+1)*mSizeMaterial;
    if(br.y > clamp)
    {
        float over = 1 - (br.y - clamp) / (br.y - ul.y);
        prop->mTexCoords[2].y = over;
        prop->mTexCoords[3].y = over;
        prop->mVertecies[2].y = clamp;
        prop->mVertecies[3].y = clamp;
    }
    
    prop->mNext = 0;
    
    Prop* cur = mProps[x + y*mSizeY];
    if(cur)
    {
       while(cur->mNext) cur = cur->mNext;
       cur->mNext = prop;
    }
    else
    {
        mProps[x + y*mSizeY] = prop;
    }
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
    Vector2 ul = (camera->mCenter - camera->mSize/2) / mSizeMaterial;
    if(ul.x < 0) ul.x = 0;
    if(ul.x > mSizeX) ul.x = mSizeX;
    if(ul.y < 0) ul.y = 0;
    if(ul.y > mSizeY) ul.y = mSizeY;
    Vector2 lr = (camera->mCenter + camera->mSize/2) / mSizeMaterial;
    if(lr.x < 0) lr.x = 0;
    if(lr.x > mSizeX) lr.x = mSizeX;
    if(lr.y < 0) lr.y = 0;
    if(lr.y > mSizeY) lr.y = mSizeY;
    
    uint16_t startX = floorf(ul.x);
    uint16_t startY = floorf(ul.y);
    uint16_t endX = ceilf(lr.x);
    uint16_t endY = ceilf(lr.y);
    
    drawTiles(startX, startY, endX, endY);
    drawProps(startX, startY, endX, endY);
    //drawEntities();
    
    camera->unset();
}

// Sets up an array of values for the texture coordinates.
const GLfloat spriteTexcoords[] = {
0.0, 0.0,
1.0, 0.0,
0.0, 1.0,
1.0, 1.0,
};

void Map::drawTiles(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    glDisable(GL_BLEND);
    glLoadIdentity();
    
    glTexCoordPointer(2, GL_FLOAT, 0, spriteTexcoords);
    
    for(uint16_t y=startY; y < endY; ++y)
    {
        Vector2 vertices[4];
        vertices[0] = Vector2(startX*mSizeMaterial, y*mSizeMaterial);
        vertices[1] = Vector2((startX+1)*mSizeMaterial, y*mSizeMaterial);
        vertices[2] = Vector2(startX*mSizeMaterial, (y+1)*mSizeMaterial);
        vertices[3] = Vector2((startX+1)*mSizeMaterial, (y+1)*mSizeMaterial);
        
        for(uint16_t x=startX; x < endX; ++x)
        {
            Image* img = mMaterials[mTiles[x + y*mSizeY]];
            //drawImage(img, curXTrans, 1);
            
            glBindTexture(GL_TEXTURE_2D, img->mTexture);
            glVertexPointer(2, GL_FLOAT, 0, (float*)vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            // Translate one tile to the right.
            vertices[0].x += mSizeMaterial;
            vertices[1].x += mSizeMaterial;
            vertices[2].x += mSizeMaterial;
            vertices[3].x += mSizeMaterial;
        }
    }
    glEnable(GL_BLEND);
}

void Map::drawProps(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY)
{
    glLoadIdentity();
    for(uint16_t y=startY; y < endY; ++y)
    {
        for(uint16_t x=startX; x < endX; ++x)
        {
            Prop* prop = mProps[x + y*mSizeY];
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
}

void Map::drawEntities()
{
    for(ImageEntityVector::iterator i=mEntities.begin(); i != mEntities.end(); ++i)
    {
        (*i)->draw();
    }
}

uint16_t Map::getSizeX() const
{
    return mSizeX;
}
uint16_t Map::getSizeY() const
{
    return mSizeY;
}

uint16_t Map::getSizeMaterial() const
{
    return mSizeMaterial;
}


}