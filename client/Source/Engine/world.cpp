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
    mLastPhase = Touch::PhaseBegin;
}

World::~World()
{

}

int World::init()
{
    int ret;
	mCamera = new Camera(Vector2(0, 0), getFrameSize());
    mVehicle = new Vehicle;
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

void World::addEntity(Entity* entity)
{
	mEntities.push_back(entity);
}

void World::setPath(Vector2Vec const& path)
{
    Vector2Vec worldPath;
    for(Vector2Vec::const_iterator i = path.begin(); i!=path.end(); ++i)
    {
        worldPath.push_back(mCamera->translateToWorld(*i));
    }
    mVehicle->setPath(worldPath);
}

void World::draw()
{
	EntityVector::iterator i;
	for(i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		(*i)->draw();
	}
    
    mVehicle->draw();

    mCamera->mCenter = mVehicle->mCenter;
    mCamera->makeDirty();
    mCamera->set();
}

void World::update(int delta)
{
    mVehicle->update(delta);
}

bool World::touch(uint32_t count, Touch* touches)
{
    // Zoom out.
    if(count == 2)
    {
        // No motion occurred.
        if(mLastPhase == Touch::PhaseBegin && touches[0].mPhase == Touch::PhaseEnd)
        {
            mLastPhase = touches[0].mPhase;
            mCamera->mSize*=2;
            return true;
        }
    }    
    // Zoom in.

    else if(count == 1 && mLastPhase == Touch::PhaseBegin && touches[0].mPhase == Touch::PhaseEnd)
    {
        mLastPhase = touches[0].mPhase;
        mCamera->mSize/=2;
        return true;
    }

//    else if(count == 1 touches[0].mPhase == Touch::PhaseMove)
//    {
        // 



    mVehicle->touch(count, touches);


    return false;
}

} //namespace pammo