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
		World(char const* mapeName);
		virtual ~World();
		
        virtual void init();

        virtual void draw();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();

        virtual uint32_t getTouchPriority() const {return 2;}
        virtual uint32_t getDrawPriority() const {return 2;}

        void setPath(Vector2Vec const& path);

		void addEntity(Entity* entity);

        void zoomOut();
        void zoomIn();
        bool isZoomedOut();
    
        ParticleSystem* getParticleSystem();
        TileMap* getTileMap();
        CollisionMap* getCollisionMap();
        CollisionDynamics* getCollisionDynamics();
        Camera* getCamera();

    protected:
    private:
        void gotoLobby();
    
		Camera* mCamera;
        Vector2 mTargetCameraSize;
        typedef vector< Entity* > EntityVector;
		EntityVector mEntities;
        EntityVector mVehicles;
        Touch::Phase mLastPhase;
        
        ImageEntity* mBackButton;
        HealthMeter* mHealthMeter;

        ParticleSystem* mParticleSystem;
        TileMap* mTileMap;
        CollisionMap* mCollisionMap;
        CollisionDynamics* mCollisionDynamics;

        Player* mPlayer;

        NpcManager* mNpcManager;

        bool mZoomedOut;
};
    
extern World* gWorld;

}

#endif
