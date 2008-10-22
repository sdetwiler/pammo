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
}
    
Map::~Map()
{
}

void Map::loadBackdrop(char const* backdropName)
{
    mBucketSizeX = 496;
    mBucketSizeY = 496;
    mBucketCountX = ceilf((float)mSizeX/mBucketSizeX);
    mBucketCountY = ceilf((float)mSizeY/mBucketSizeY);
    
    assert(mBucketCountX == MAP_TILES_X);
    assert(mBucketCountY == MAP_TILES_Y);

    mPreview = gImageLibrary->getImage(MAP_PREVIEW);
    
    uint32_t previousYCoord = 8;
    for(uint32_t y=0; y < mBucketCountY; ++y)
    {
        uint32_t currentYCoord = previousYCoord + mBucketSizeY;
        uint32_t previousXCoord = 8;
        for(uint32_t x=0; x < mBucketCountX; ++x)
        {
            uint32_t currentXCoord = previousXCoord + mBucketSizeX;
            
            // Create the prop.
            Prop* prop = &mBuckets[x + y*mBucketCountX];
            prop->mImage = 0;
            prop->mId = MAP_TILES_BASE + x + y*mBucketCountX;
    
            // Calculate preview tex coordinates.
            float left = (float)previousXCoord / mSizeX;
            float right = (float)currentXCoord / mSizeX;
            float top = (float)previousYCoord / mSizeY;
            float bottom = (float)currentYCoord / mSizeY;
            prop->mPreviewTexCoords[0] = Vector2(left, top);
            prop->mPreviewTexCoords[1] = Vector2(right, top);
            prop->mPreviewTexCoords[2] = Vector2(left, bottom);
            prop->mPreviewTexCoords[3] = Vector2(right, bottom);
    
            // Calculate regular tex coords.
            float lo = 8 / 512.;
            float hi = 504 / 512.;
            prop->mTexCoords[0] = Vector2(lo, lo);
            prop->mTexCoords[1] = Vector2(hi, lo);
            prop->mTexCoords[2] = Vector2(lo, hi);
            prop->mTexCoords[3] = Vector2(hi, hi);
    
            // Calculate verticies.
            prop->mVertecies[0] = Vector2(previousXCoord, previousYCoord);
            prop->mVertecies[1] = Vector2(currentXCoord,  previousYCoord);
            prop->mVertecies[2] = Vector2(previousXCoord, currentYCoord);
            prop->mVertecies[3] = Vector2(currentXCoord,  currentYCoord);
            
            previousXCoord = currentXCoord;
        }
        previousYCoord = currentYCoord;
    }
}

uint32_t Map::getDrawPriority() const
{
    return kMapPriority;
}
    
void Map::draw()
{
    glDisable(GL_BLEND);

    Camera* camera = gWorld->getCamera();
    camera->set();
    
    // Calculate lower-right and upper-left corners of camera view.
    assert(camera->mRotation == 0);
    Vector2 ul = camera->mCenter - camera->mSize/2 - Vector2(8, 8);
    Vector2 lr = camera->mCenter + camera->mSize/2 - Vector2(8, 8);
    
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
    // Draw surrounding tiles.
    for(uint32_t y=startY; y < endY; ++y)
    {
        for(uint32_t x=startX; x < endX; ++x)
        {
            Prop* prop = &mBuckets[x + y*mBucketCountX];
            
            // If image doesn't exist, dispatch decode.
            if(prop->mImage == 0)
                prop->mImage = gImageLibrary->tryGetImage(prop->mId);
                //prop->mImage = gImageLibrary->getImage(prop->mId);
            
            // If the image doesn't exist, bind the preview image and tex coords.
            if(prop->mImage == 0)
            {
                glTexCoordPointer(2, GL_FLOAT, 0, (float*)prop->mPreviewTexCoords);
                glBindTexture(GL_TEXTURE_2D, mPreview->mTexture);
            }
            else
            {
                glTexCoordPointer(2, GL_FLOAT, 0, (float*)prop->mTexCoords);
                glBindTexture(GL_TEXTURE_2D, prop->mImage->mTexture);
            }
        
            // Draw.
            glVertexPointer(2, GL_FLOAT, 0, (float*)prop->mVertecies);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
    
    camera->unset();
    
    glEnable(GL_BLEND);
    
    // Disable pre-hinting
    #if 0
    
    // Prehint, based on where the camera is in the main tile.
    bool hintLeft = false, hintUp = false, hintRight = false, hintDown = false;
    
    // Determin left / right hint.
    if((uint32_t)camera->mCenter.x % mBucketSizeX > mBucketSizeX / 2)
    {
        if(startX > 1)
        {
            //dprintf("Prehinting in -X");
            hintLeft = true;
            
            // Hint the column to the left.
            for(uint32_t y = startY; y < endY; ++y)
            {
                Prop* prop = mBuckets[startX-1 + y*mBucketCountX];
                if(prop->mImage == 0)
                    prop->mImage = gImageLibrary->tryGetImage(prop->mId);
            }
        }
        else
        {
            //dprintf("Not prehinting in -X");
        }
    }
    else
    {
        if(endX < mBucketCountX - 2)
        {
            //dprintf("Prehinting in +X");
            hintRight = true;
            
            // Hint the column to the right.
            for(uint32_t y = startY; y < endY; ++y)
            {
                Prop* prop = mBuckets[endX+1 + y*mBucketCountX];
                if(prop->mImage == 0)
                    prop->mImage = gImageLibrary->tryGetImage(prop->mId);
            }
        }
        else
        {
            //dprintf("Not prehinting in +X");
        }
    }
    
    // Determin up  down hint.
    if((uint32_t)camera->mCenter.y % mBucketSizeY > mBucketSizeY / 2)
    {
        if(startY > 1)
        {
            //dprintf("Prehinting in -Y");
            hintUp = true;
            
            // Hint the row above.
            for(uint32_t x = startX; x < endX; ++x)
            {
                Prop* prop = mBuckets[x + (startY-1)*mBucketCountX];
                if(prop->mImage == 0)
                    prop->mImage = gImageLibrary->tryGetImage(prop->mId);
            }
        }
        else
        {
            //dprintf("Not prehinting in -Y");
        }
    }
    else
    {
        if(endY < mBucketCountY - 2)
        {
            //dprintf("Prehinting in +Y");
            hintDown = true;
            
            // Hint the row below.
            for(uint32_t x = startX; x < endX; ++x)
            {
                Prop* prop = mBuckets[x + (endY+1)*mBucketCountX];
                if(prop->mImage == 0)
                    prop->mImage = gImageLibrary->tryGetImage(prop->mId);
            }
        }
        else
        {
            //dprintf("Not prehinting in +Y");
        }
    }
    
    // Prehint diagonals.
    if(hintLeft && hintUp)
    {
        Prop* prop = mBuckets[startX-1 + (startY-1)*mBucketCountX];
        if(prop->mImage == 0)
            prop->mImage = gImageLibrary->tryGetImage(prop->mId);
    }
    if(hintRight && hintUp)
    {
        Prop* prop = mBuckets[endX+1 + (startY-1)*mBucketCountX];
        if(prop->mImage == 0)
            prop->mImage = gImageLibrary->tryGetImage(prop->mId);
    }
    if(hintLeft && hintDown)
    {
        Prop* prop = mBuckets[startX-1 + (endY+1)*mBucketCountX];
        if(prop->mImage == 0)
            prop->mImage = gImageLibrary->tryGetImage(prop->mId);
    }
    if(hintRight && hintDown)
    {
        Prop* prop = mBuckets[endX+1 + (endY+1)*mBucketCountX];
        if(prop->mImage == 0)
            prop->mImage = gImageLibrary->tryGetImage(prop->mId);
    }
    #endif
}

void Map::lowMemory()
{
    // Code copied from draw.
    Camera* camera = gWorld->getCamera();
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
    
    for(uint32_t x=0; x < mBucketCountX; ++x)
    {
        for(uint32_t y=0; y < mBucketCountY; ++y)
        {
            if(x >= startX && x <= endX && y >= startY && y <= endY)
                continue;
                
            Prop* prop = &mBuckets[x + y*mBucketCountX];
            if(prop->mImage)
            {
                gImageLibrary->purgeImage(prop->mImage);
                prop->mImage = 0;
            }
        }
    }
}

void Map::setSize(Vector2 const& size)
{
    mSizeX = size.x;
    mSizeY = size.y;
}

Vector2 Map::getSize() const
{
    return Vector2(mSizeX, mSizeY);
}

#if 0
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
#endif

}