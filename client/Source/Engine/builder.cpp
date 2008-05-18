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
#include "graphicsContext.h"

namespace pammo
{

#if 0
void builder(World* world)
{
	GraphicsContext* context = world->mContext;
	Entity* entity;
	
	entity = new Entity(context->openImage("ground.png"));
	entity->mCenter.x = 0;
	entity->mCenter.y = 0;
	entity->mSize.x = 2048;
	entity->mSize.y = 2048;
	world->addEntity(entity);
}

#else

void builder(World* world)
{
	GraphicsContext* context = world->mContext;
	Entity* entity;
	
	entity = new Entity(context->openImage("ground.png"));
	entity->mSize.x = 2048;
	entity->mSize.y = 2048;
	world->addEntity(entity);
	
	entity = new Entity(context->openImage("MagentaButton.png"));
	world->addEntity(entity);
	entity->mSize.x = 512;
	entity->mSize.y = 512;
	entity->mCenter.x = 512;
	
	entity = new Entity(context->openImage("MagentaButton.png"));
	world->addEntity(entity);
	entity->mCenter.x = 200;
	entity->mRotation = 0.7;
	
	entity = new Entity(context->openImage("CyanButton.png"));
	world->addEntity(entity);
	entity->mSize.x = 512;
	entity->mSize.y = 512;
	entity->mCenter.y = -512;
	
	entity = new Entity(context->openImage("CyanButton.png"));
	world->addEntity(entity);
	entity->mCenter.y = -200;
	entity->mRotation = 0.7;
	
	entity = new Entity(context->openImage("YellowButton.png"));
	world->addEntity(entity);
	entity->mSize.x = 40;
	entity->mSize.y = 40;
	entity->mCenter.x = 100;
	entity->mCenter.y = 100;
	
	entity = new Entity(context->openImage("car.png"));
	world->addEntity(entity);
}

#endif

}
