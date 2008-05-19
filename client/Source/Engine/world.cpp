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
#include "image.h"

namespace pammo
{

World::World(Game* game)
{
	mGame = game;
	mCamera = new Camera(Vector2(0, 0), getFrameSize());
}

World::~World()
{

}
	
uint32_t World::getDrawPriority() const
{
    return 2;
}

uint32_t World::getTouchPriority() const
{
    return 2;
}

void World::addEntity(Entity* entity)
{
	mEntities.push_back(entity);
}

void World::draw()
{
	mCamera->set();
	
	EntityVector::iterator i;
	for(i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		(*i)->draw();
	}
}

void World::update(int delta)
{
}

bool World::touch(uint32_t count, Touch* touches)
{
    for(uint32_t i=0; i<count; ++i)
    {
        if(touches[i].mPhase == Touch::PhaseBegin)
        {
            //mStartTouch = touches[i].mLocation;
        }

        else if(touches[i].mPhase == Touch::PhaseMove)
        {
//            mCamera->mCenter.x -= touches[i].mLocation.x - mStartTouch.x;
//            mCamera->mCenter.y -= touches[i].mLocation.y - mStartTouch.y;
//            mCamera->makeDirty();
//            mStartTouch = touches[i].mLocation;
        }
    }

    return false;
}

} //namespace pammo