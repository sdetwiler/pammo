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

namespace pammo
{

class Entity;
class Camera;
class TileMap;
class PathManager;

class World : public View
{
	public:
		World(char const* mapeName);
		virtual ~World();
		
        virtual void init();

        virtual void draw();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update(int delta);

        virtual uint32_t getTouchPriority() const {return 2;}
        virtual uint32_t getDrawPriority() const {return 2;}

        void setPath(Vector2Vec const& path);

		void addEntity(Entity* entity);

        void zoomOut();
        void zoomIn();
        bool isZoomedOut();
    
        ParticleSystem* getParticleSystem();
        TileMap* getTileMap();
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

        ParticleSystem* mParticleSystem;
        TileMap* mTileMap;

        Player* mPlayer;

        bool mZoomedOut;
};
    
extern World* gWorld;

}

#endif
