#ifndef __WORLD_H__
#define __WORLD_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Camera;
class Map;
class Player;
class ParticleSystem;
class Physics;
class Minimap;
class InterfaceView;

class EnemyManager;

class World : public View
{
	public:
		World();
		~World();
        
        Camera* getCamera();
        Map* getMap();
        ParticleSystem* getParticleSystem();
        Physics* getPhysics();
        Minimap* getMinimap();
        Player* getPlayer() const;
        EnemyManager* getEnemyManager() const;

        virtual void destroy();

    protected:
    private:
		friend class Player; // SCD: continuation of horrible hack.
		Camera* mCamera;
        Map* mMap;
        ParticleSystem* mParticleSystem;
        Physics* mPhysics;
        Minimap* mMinimap;
        
        InterfaceView* mInterfaceView;

        Player* mPlayer;
        EnemyManager* mEnemyManager;
};
    
extern World* gWorld;

}

#endif
