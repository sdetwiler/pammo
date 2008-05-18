/*
 *  world.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "world.h"
#include "camera.h"
#include "entity.h"
#include "graphicsContext.h"

World::World(GraphicsContext* context)
{
	mContext = context;
	mCamera = new Camera(Vector2(0, 0), context->getFrameSize());
}

World::~World()
{

}
		
void World::addEntity(Entity* entity)
{
	mEntities.push_back(entity);
}
		
void World::draw()
{
	mContext->frameBegin();
	
	mCamera->set(mContext);
	
	EntityVector::iterator i;
	for(i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		(*i)->draw(mContext);
	}
	
	mContext->frameEnd();
}

void World::update(int delta)
{
}

void World::touches(uint32_t count, Touch* touches)
{
    for(uint32_t i=0; i<count; ++i)
    {
        if(touches[i].mPhase == Touch::TouchPhaseBegin)
        {
            mStartTouch = touches[i].mLocation;
        }

        else if(touches[i].mPhase == Touch::TouchPhaseMove)
        {
            mCamera->mCenter.x -= touches[i].mLocation.x - mStartTouch.x;
            mCamera->mCenter.y -= touches[i].mLocation.y - mStartTouch.y;
            mCamera->makeDirty();
            mStartTouch = touches[i].mLocation;
        }
    }
}