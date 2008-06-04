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
#include "flameTankVehicle.h"

namespace pammo
{
    
World* gWorld = NULL;

World::World()
{
    mZoomedOut = false;
    mLastPhase = Touch::PhaseBegin;
    
    gGame->registerDrawable(this);
    gGame->registerTouchable(this);
    gGame->registerUpdateable(this);
}

World::~World()
{
}

int World::init()
{
    int ret;
    mParticleSystem = new ParticleSystem(1000);

    mCamera = new Camera(Vector2(0, 0), getFrameSize());
    mCamera->mSize*=2;
    mTargetCameraSize = mCamera->mSize;
    mVehicle = new FlameTankVehicle(this);

    ret = mVehicle->init();
    if(ret < 0)
        return ret;

    return 0;
}
	
uint32_t World::getDrawPriority() const
{
    return 2;
}

uint32_t World::getTouchPriority() const
{
    return 2;
}

ParticleSystem* World::getParticleSystem()
{
    return mParticleSystem;
}

void World::addEntity(Entity* entity)
{
	mEntities.push_back(entity);
}

void World::setPath(Vector2Vec const& path)
{
    zoomIn();

    Vector2Vec worldPath;
    for(Vector2Vec::const_iterator i = path.begin(); i!=path.end(); ++i)
    {
        worldPath.push_back(mCamera->translateToWorld(*i));
    }
    mVehicle->setPath(worldPath);
}

void World::draw()
{
		
	// Zoom the camera if required.
	if(mTargetCameraSize != mCamera->mSize)
	{
		Vector2 step = (mCamera->mSize - mTargetCameraSize)/4.0;
		if(fabs(step.x) < 0.5)
		{
			mCamera->mSize = mTargetCameraSize;
		}
		else
		{
			mCamera->mSize-= step;
		}
	}
	mCamera->mCenter = mVehicle->mCenter;
	mCamera->makeDirty();
	mCamera->set();
		
	EntityVector::iterator i;
	for(i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		(*i)->draw();
	}    
    mVehicle->draw();
    mParticleSystem->draw();	
	mCamera->unset();
}

void World::update(int delta)
{
    mParticleSystem->update();
    mVehicle->update(delta);
}

bool World::isZoomedOut()
{
    return mZoomedOut;
}

void World::zoomOut()
{
    if(mZoomedOut)
        return;

    mTargetCameraSize = mCamera->mSize*4.0;
    mZoomedOut = true;
}

void World::zoomIn()
{
    if(!mZoomedOut)
        return;

    mTargetCameraSize = mCamera->mSize/4.0;
    mZoomedOut = false;
}

bool World::touch(uint32_t count, Touch* touches)
{
    // Two finger touch: zoom out.
    if(count == 2)
    {
        // No motion occurred.
        if(mLastPhase == Touch::PhaseBegin && touches[0].mPhase == Touch::PhaseEnd)
        {
            if(!mZoomedOut)
            {
                mLastPhase = touches[0].mPhase;
                zoomOut();
            }
            return true;
        }
    }    

    // One finger touch: zoom in.
    else if(count == 1)
    {
        // No motion occured.
        if(mLastPhase == Touch::PhaseBegin && touches[0].mPhase == Touch::PhaseEnd)
        {
            if(mZoomedOut)
            {
                mLastPhase = touches[0].mPhase;
                zoomIn();
            }
            return true;
        }
    }

    mLastPhase = touches[0].mPhase;

    mVehicle->touch(count, touches);

    return false;
}

} //namespace pammo
