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

namespace pammo
{

class Entity;
class Camera;
class TileMap;
class PathManager;

class World : 
    public Drawable,
    public Touchable,
    public Updateable
{
	public:
		World();
		~World();
		
        int init(char const* mapName);

        void setPath(Vector2Vec const& path);

        virtual void draw();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update(int delta);

        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getDrawPriority() const;

		void addEntity(Entity* entity);

        void zoomOut();
        void zoomIn();
        bool isZoomedOut();
    
        ParticleSystem* getParticleSystem();
        TileMap* getTileMap();

    protected:
    private:
		Camera* mCamera;
        Vector2 mTargetCameraSize;
        typedef vector< Entity* > EntityVector;
		EntityVector mEntities;
        Vehicle* mVehicle;
        EntityVector mVehicles;
        Touch::Phase mLastPhase;

        ParticleSystem* mParticleSystem;
        TileMap* mTileMap;
        PathManager* mPathManager;

        bool mZoomedOut;
};
    
extern World* gWorld;

}

#endif
