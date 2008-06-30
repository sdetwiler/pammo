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
#include "builder.h"
#include "tileMap.h"
#include "player.h"
#include "lobbyView.h"
#include "imageLibrary.h"
#include "collisionMap.h"
#include "collisionDynamics.h"

#include "vehicleSelectView.h"

namespace pammo
{
    
World* gWorld = NULL;

World::World(char const* mapName, uint32_t vehicleType)
{
    gWorld = this;

    int ret;
    gGame->queueInitable(this);
    
    mZoomedOut = false;
    mLastPhase = Touch::PhaseBegin;

    mCamera = new Camera(Vector2(0, 0), getFrameSize());
    mTargetCameraSize = mCamera->mSize;
    
    mParticleSystem = new ParticleSystem(200);
    mTileMap = new TileMap;
    mCollisionMap = new CollisionMap;
    mCollisionDynamics = new CollisionDynamics;
    
    buildFromMap(this, mapName);
    
    mPlayer = new Player;
    mPlayer->setVehicleType(vehicleType);
    ret = mPlayer->init(Player::Local);
    assert(ret == 0);

    mNpcManager = new NpcManager;
    ret = mNpcManager->init(3);
    assert(ret == 0);


    mBackButton = new ImageEntity(gImageLibrary->reference("data/interface/Back.png"));
    mBackButton->mCenter = Vector2(mBackButton->mSize/2 + Vector2(32, 32));

    mHealthMeter = new HealthMeter;
    mHealthMeter->init();
    mPlayer->setObserver(mHealthMeter);
}

World::~World()
{
    delete mCollisionMap;
    delete mPlayer;
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

void World::setVehicleType(uint32_t type)
{
    mPlayer->setVehicleType(type);
}

ParticleSystem* World::getParticleSystem()
{
    return mParticleSystem;
}

Camera* World::getCamera()
{
    return mCamera;
}
    
TileMap* World::getTileMap()
{
    return mTileMap;
}

CollisionMap* World::getCollisionMap()
{
    return mCollisionMap;
}

CollisionDynamics* World::getCollisionDynamics()
{
    return mCollisionDynamics;
}

void World::addEntity(Entity* entity)
{
	mEntities.push_back(entity);
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
    mPlayer->draw();
    
    mNpcManager->draw();

    mParticleSystem->draw();
    // SCD mCollisionMap->draw();
	mCamera->unset();
    
    mBackButton->draw();
    mHealthMeter->draw();
}

void World::update()
{
    mParticleSystem->update();
    mPlayer->update();
    mNpcManager->update();
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

    // If the player is not destroyed move the camera, else hold current position.
    if(mPlayer->getState() != Player::Destroyed)
	    mCamera->mCenter = mPlayer->getCenter();

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

    return mPlayer->touch(count, touches);
}

void World::gotoLobby()
{
    // Create lobby.
    new LobbyView();
    
    // Delete self.
    gGame->queueDeleteable(this);
}

} //namespace pammo
