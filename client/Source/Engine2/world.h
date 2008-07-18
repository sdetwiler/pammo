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

namespace pammo
{

class Camera;
class Map;
class Player;
class ParticleSystem;

class World
{
	public:
		World();
		~World();
        
        Camera* getCamera();
        Map* getMap();
        ParticleSystem* getParticleSystem();

    protected:
    private:
		Camera* mCamera;
        Map* mMap;
        ParticleSystem* mParticleSystem;
        
        Player* mPlayer;
};
    
extern World* gWorld;

}

#endif
