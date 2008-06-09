/*
 *  tileMap.h
 *  pammo
 *
 *  Created by James Marr on 6/3/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "pammo.h"

namespace pammo
{
    
class Camera;
struct Image;
    
class TileMap
{
public:
    TileMap();
    ~TileMap();
    
    void setNumMaterials(uint16_t numMaterials, uint16_t sizeMaterial = 128);
    void setMaterial(uint16_t index, char const* materialPath);
    
    void setNumTiles(uint16_t sizeX, uint16_t sizeY);
    void setTile(uint16_t x, uint16_t y, uint16_t index);
    
    void draw(Camera* camera);
    
protected:
    uint16_t mNumMaterials;
    Image** mMaterials;
    uint16_t mSizeMaterial;
    uint16_t mSizeX;
    uint16_t mSizeY;
    uint16_t* mTiles;
};

}
#endif