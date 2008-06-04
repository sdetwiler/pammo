/*
 *  builder.cpp
 *  Flain
 *
 *  Created by James Marr on 4/1/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "builder.h"
#include "world.h"
#include "imageEntity.h"
#include "imageLibrary.h"
#include "tileMap.h"

namespace pammo
{
    
uint16_t readUInt16(char** cur, size_t* remain)
{
    uint16_t tmp;
    assert(*remain >= sizeof(tmp));
    memcpy(&tmp, *cur, sizeof(tmp));
    *cur += sizeof(tmp);
    *remain -= sizeof(tmp);
    return ntohs(tmp);
}
    
float readFloat(char** cur, size_t* remain)
{
    float tmp;
    assert(*remain >= sizeof(tmp));
    memcpy(&tmp, *cur, sizeof(tmp));
    *cur += sizeof(tmp);
    *remain -= sizeof(tmp);
    
    uint32_t* ptr = (uint32_t*)&tmp;
    *ptr = ntohl(*ptr);
    return tmp;
}
    
char* readString(char** cur, size_t* remain)
{
    char* tmp = *cur;
    size_t len = 0;
    while((*cur)[len] != 0)
    {
        ++len;
        assert(*remain >= len + 1);
    }
    assert(*remain >= len + 1);
    *cur += len+1;
    *remain -= len+1;
    return tmp;
}
	
void buildFromMap(World* world, char const* name)
{
    string fullName = string("data/bmaps/") + name + ".bmap";
	FILE* f = fopen(fullName.c_str(), "rb");
	if(!f)
	{
		dprintf("Error opening map %s", name);
		return;
	}
    
    // Read the entire map into memory.
    fseek(f, 0, SEEK_END);
    size_t remain = ftell(f);
    rewind(f);
    char* buffer = new char[remain];
    fread(buffer, remain, 1, f);
    fclose(f);
    char* cur = buffer;
    
    // Get a pointer to the tileMap to fill in as we go... in that style.
    TileMap* tileMap = world->getTileMap();
    
	// Read num materials.
	uint16_t numMaterials = readUInt16(&cur, &remain);
    tileMap->setNumMaterials(numMaterials);
	dprintf("Materials: %d", numMaterials);
	
    // Load each material.
	for(uint32_t i=0; i < numMaterials; ++i)
	{
        char* materialName = readString(&cur, &remain);
        string materialPath = string("data/materials/") + materialName + ".png";
        tileMap->setMaterial(i, materialPath.c_str());
        dprintf("%d - %s", i, materialPath.c_str());
	}
    
    // Load each tile.
    uint16_t tilesX = readUInt16(&cur, &remain);
    uint16_t tilesY = readUInt16(&cur, &remain);
    tileMap->setNumTiles(tilesX, tilesY);
    for(uint16_t y=0; y < tilesY; ++y)
    {
        for(uint16_t x=0; x < tilesX; ++x)
        {
            uint16_t i = readUInt16(&cur, &remain);
            tileMap->setTile(x, y, i);
        }
    }
    
	// Read num props.
	uint16_t numProps = readUInt16(&cur, &remain);
	dprintf("Props: %d", numProps);
    Image** propLookup = new Image*[numProps];

    // Load each prop.
	for(uint32_t i=0; i < numProps; ++i)
	{
        char* propName = readString(&cur, &remain);
        string propPath = string("data/props/") + propName + ".png";
        propLookup[i] = gImageLibrary->reference(propPath.c_str());
        dprintf("%d - %s", i, propPath.c_str());
	}
    
    // Load each entity.
    uint16_t numEntities = readUInt16(&cur, &remain);
    for(uint16_t tmp=0; tmp < numEntities; ++tmp)
    {
        uint16_t i = readUInt16(&cur, &remain);
        dprintf("Prop using %d", i);
        float posX = readFloat(&cur, &remain);
        float posY = readFloat(&cur, &remain);
        float scale = readFloat(&cur, &remain);
        float rot = readFloat(&cur, &remain);
        
        if(i>numProps)
        {
            dprintf("Error in map format: prop %d is out of bounds. %d is max %d.\n", i, numProps, htons(i));
            continue;
        }

        if(propLookup[i])
        {
            ImageEntity* e = new ImageEntity(propLookup[i]);
            e->mCenter = Vector2(posX, posY);
            e->mSize *= scale;
            e->mRotation = rot;
            world->addEntity(e);
        }
    }
    
    // Cleanup.
    delete[] buffer;
}
    
    
    
#if 0
	
void builder(World* world)
{
	Entity* entity;
	
	entity = new ImageEntity(openImage("data/misc/ground.png"));
	entity->mCenter.x = 0;
	entity->mCenter.y = 0;
	entity->mSize.x = 2048;
	entity->mSize.y = 2048;
	world->addEntity(entity);
}

#else

void builder(World* world)
{
	ImageEntity* entity;
	
    Image* image;
    image = openImage("data/misc/ground.png");
	if(image)
    {
        entity = new ImageEntity(image);
	    entity->mSize.x = 2048;
	    entity->mSize.y = 2048;
        world->addEntity(entity);
    }
	
    image = openImage("data/props/smallTent.png");
    if(image)
    {
        entity = new ImageEntity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 512;
	    entity->mSize.y = 512;
	    entity->mCenter.x = 512;
    }

    image = openImage("data/props/smallTent.png");
    if(image)
    {
        entity = new ImageEntity(image);
	    world->addEntity(entity);
	    entity->mCenter.x = (float)200;
	    entity->mRotation = (float)0.7;
    }

    image = openImage("data/props/cratesWIP2.png");
    if(image)
    {
        entity = new ImageEntity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 512;
	    entity->mSize.y = 512;
	    entity->mCenter.y = -512;
    }	
    image = openImage("data/props/smallTent.png");
    if(image)
    {
        entity = new ImageEntity(image);
	    world->addEntity(entity);
	    entity->mCenter.y = -200;
	    entity->mRotation = (float)0.7;
    }	
    image = openImage("data/props/cratesWIP2.png");
    if(image)
    {
        entity = new ImageEntity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 40;
	    entity->mSize.y = 40;
	    entity->mCenter.x = 100;
	    entity->mCenter.y = 100;
    }	
}

#endif

}
