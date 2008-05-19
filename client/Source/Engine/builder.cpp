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
#include "entity.h"
#include "image.h"

namespace pammo
{

#if 0
void builder(World* world)
{
	Entity* entity;
	
	entity = new Entity(openImage("ground.png"));
	entity->mCenter.x = 0;
	entity->mCenter.y = 0;
	entity->mSize.x = 2048;
	entity->mSize.y = 2048;
	world->addEntity(entity);
}

#else

void builder(World* world)
{
	Entity* entity;
	
    Image* image;
    image = openImage("ground.png");
	if(image)
    {
        entity = new Entity(image);
	    entity->mSize.x = 2048;
	    entity->mSize.y = 2048;
        world->addEntity(entity);
    }
	
    image = openImage("MagentaButton.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 512;
	    entity->mSize.y = 512;
	    entity->mCenter.x = 512;
    }

    image = openImage("MagentaButton.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
	    entity->mCenter.x = 200;
	    entity->mRotation = 0.7;
    }

    image = openImage("CyanButton.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 512;
	    entity->mSize.y = 512;
	    entity->mCenter.y = -512;
    }	
    image = openImage("CyanButton.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
	    entity->mCenter.y = -200;
	    entity->mRotation = 0.7;
    }	
    image = openImage("YellowButton.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
	    entity->mSize.x = 40;
	    entity->mSize.y = 40;
	    entity->mCenter.x = 100;
	    entity->mCenter.y = 100;
    }	
    image = openImage("flameTank.png");
    if(image)
    {
        entity = new Entity(image);
	    world->addEntity(entity);
    }
}

#endif

}
