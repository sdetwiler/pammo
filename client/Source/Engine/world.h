/*
 *  world.h
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include "pammo.h"
#include "widget.h"
#include "vehicle.h"
#include "particleSystem.h"
#include "player.h"

#include "healthMeter.h"

#include "npcManager.h"

namespace pammo
{

class Entity;
class Camera;
class TileMap;
class PathManager;
class CollisionMap;
class CollisionDynamics;

class World : public View
{
	public:
		World(char const* mapeName, uint32_t vehicleType);
		virtual ~World();
		
        virtual void init();

        virtual void draw();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();

        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getDrawPriority() const;

        void setPath(Vector2Vec const& path);

		void addEntity(Entity* entity);

        void zoomOut();
        void zoomIn();
        bool isZoomedOut();
    
        void setVehicleType(uint32_t type);

        ParticleSystem* getParticleSystem();
        TileMap* getTileMap();
        CollisionMap* getCollisionMap();
        CollisionDynamics* getCollisionDynamics();
        Camera* getCamera();
        
        void addSpawnPoint(Vector2 spawnPoint);
        Vector2 getRandomSpawnPoint();

    protected:
    private:
        void gotoLobby();
    
		Camera* mCamera;
        Vector2 mTargetCameraSize;
        typedef vector< Entity* > EntityVector;
		EntityVector mEntities;
        EntityVector mVehicles;
        Touch::Phase mLastPhase;
        
        vector< Vector2 > mSpawnPoints;
        
        ImageEntity* mBackButton;
        HealthMeter* mHealthMeter;

        ParticleSystem* mParticleSystem;
        TileMap* mTileMap;
        CollisionMap* mCollisionMap;
        CollisionDynamics* mCollisionDynamics;

        Player* mPlayer;  // Local player.

        NpcManager* mNpcManager;

        bool mZoomedOut;
};
    
extern World* gWorld;

}

#endif
