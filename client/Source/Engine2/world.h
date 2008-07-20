#ifndef __WORLD_H__
#define __WORLD_H__

#include "pammo.h"

namespace pammo
{

class Camera;
class Map;
class Player;
class ParticleSystem;
class Physics;

class World
{
	public:
		World();
		~World();
        
        Camera* getCamera();
        Map* getMap();
        ParticleSystem* getParticleSystem();
        Physics* getPhysics();

    protected:
    private:
		Camera* mCamera;
        Map* mMap;
        ParticleSystem* mParticleSystem;
        Physics* mPhysics;
        
        Player* mPlayer;
};
    
extern World* gWorld;

}

#endif
