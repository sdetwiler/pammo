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
#include "builder.h"
#include "tileMap.h"
#include "pathManager.h"
#include "lobbyView.h"
#include "imageLibrary.h"

namespace pammo
{
    
World* gWorld = NULL;

World::World(char const* mapName)
{
    gWorld = this;

    int ret;
    gGame->queueInitable(this);
    
    mZoomedOut = false;
    mLastPhase = Touch::PhaseBegin;

    mCamera = new Camera(Vector2(0, 0), getFrameSize());
    mCamera->mSize*=2;
    mTargetCameraSize = mCamera->mSize;
    
    mParticleSystem = new ParticleSystem(1000);
    mTileMap = new TileMap;
    mPathManager = new PathManager();
    
    buildFromMap(this, mapName);
    
    mVehicle = new FlameTankVehicle(this);
    ret = mVehicle->init();
    assert(ret == 0);
    
    mBackButton = new ImageEntity(gImageLibrary->reference("data/interface/Back.png"));
    mBackButton->mCenter = Vector2(mBackButton->mSize/2 + Vector2(32, 32));
}

World::~World()
{
    delete mVehicle;
    delete mPathManager;
    delete mTileMap;
    delete mParticleSystem;
    delete mCamera;
    
    gGame->unregisterDrawable(this);
    gGame->unregisterTouchable(this);
    gGame->unregisterUpdateable(this);
    
    gWorld = NULL;
}

void World::init(void)
{
    gGame->registerDrawable(this);
    gGame->registerTouchable(this);
    gGame->registerUpdateable(this);
}

ParticleSystem* World::getParticleSystem()
{
    return mParticleSystem;
}
    
TileMap* World::getTileMap()
{
    return mTileMap;
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
	mCamera->set();
    
    mTileMap->draw(mCamera);
		
	EntityVector::iterator i;
	for(i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		(*i)->draw();
	}
    mVehicle->draw();
    mParticleSystem->draw();	
	mCamera->unset();
    
    mBackButton->draw();
}

void World::update(int delta)
{
    mParticleSystem->update();
    mVehicle->update(delta);
    
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
    // Check if the user clicked in the back button.
    if(count >= 1)
    {
        Vector2 pos = touches[0].mLocation;
        Vector2 ul = mBackButton->mCenter - mBackButton->mSize/2;
        Vector2 lr = mBackButton->mCenter + mBackButton->mSize/2;
        
        if(pos.x > ul.x && pos.y > ul.y
            && pos.x < lr.x && pos.y < lr.y)
        {
            gotoLobby();
            return true;
        }
    }
    
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

void World::gotoLobby()
{
    // Create lobby.
    new LobbyView();
    
    // Delete self.
    gGame->queueDeleteable(this);
}

} //namespace pammo
