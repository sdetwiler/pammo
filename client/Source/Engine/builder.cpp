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
#include "image.h"

namespace pammo
{
	
#if 0
	
void buildFromMap(World* world, char const* name)
{
	FILE* f = fopen(name, "r");
	
	uint16_t numMaterials;
	fread((void*)&numMaterials, sizeof(numMaterials), 1, f);
	dprintf("%d", numMaterials);
}
	
void builder(World* world)
{
	buildFromMap(world, "./data/maps/Desert.bmap");
}

#elif 0
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
