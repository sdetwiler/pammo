/*
 *  tileMap.cpp
 *  pammo
 *
 *  Created by James Marr on 6/3/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "tileMap.h"

#include "camera.h"
#include "image.h"
#include "imageLibrary.h"

namespace pammo
{
    
TileMap::TileMap()
{
    mNumMaterials = 0;
    mMaterials = 0;
    mSizeMaterial = 0;
    mSizeX = 0;
    mSizeY = 0;
    mTiles = NULL;
}
    
TileMap::~TileMap()
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

void TileMap::setNumMaterials(uint16_t numMaterials, uint16_t sizeMaterial)
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
    
void TileMap::setMaterial(uint16_t index, char const* materialPath)
{
    // Verify that this slot hasn't been set yet.
    assert(mMaterials[index] == 0);
    
    Image* img = gImageLibrary->reference(materialPath);
    assert(img != 0);
    mMaterials[index] = img;
}
    
void TileMap::setNumTiles(uint16_t sizeX, uint16_t sizeY)
{
    assert(mSizeX == 0);
    assert(mSizeY == 0);
    assert(mTiles == NULL);
    
    mSizeX = sizeX;
    mSizeY = sizeY;
    
    mTiles = new uint16_t[sizeX*sizeY];
    memset(mTiles, 0, sizeof(uint16_t)*sizeX*sizeY);
}
    
void TileMap::setTile(uint16_t x, uint16_t y, uint16_t index)
{
    assert(x < mSizeX);
    assert(y < mSizeY);
    assert(index < mNumMaterials);
    
    mTiles[x + y*mSizeY] = index;
}
    
void TileMap::draw(Camera* camera)
{
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
    
    float offset = 0.5;
    
    Transform2 curYTrans = Transform2::createTranslation(Vector2(startX*mSizeMaterial - offset/2, startY*mSizeMaterial - offset/2)) *
                           Transform2::createScale(Vector2(mSizeMaterial + offset, mSizeMaterial + offset));
    Transform2 curXTrans;
    
    for(uint16_t y=startY; y < endY; ++y)
    {
        curXTrans = curYTrans;
        for(uint16_t x=startX; x < endX; ++x)
        {
            Image* img = mMaterials[mTiles[x + y*mSizeY]];
            drawImage(img, curXTrans, 1);
            
            // Translate one tile to the right.
            curXTrans[4] += mSizeMaterial;
        }
        // Translate one tile down.
        curYTrans[5] += mSizeMaterial;
    }
}
    
}