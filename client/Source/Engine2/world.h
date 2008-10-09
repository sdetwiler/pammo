#ifndef __WORLD_H__
#define __WORLD_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Camera;
class Map;
class Player;
class InfastructureManager;
class ParticleSystem;
class Physics;
class Minimap;
class InterfaceView;
class EnemyManager;

#ifdef PROFILE
class DebugScreenView;
#endif

class World
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
        InfastructureManager* getInfastructureManager() const;
        EnemyManager* getEnemyManager() const;
        
        void reset();
        void disable();
        void enable();
//        virtual void destroy();

    protected:
    private:
		friend class Player; // SCD: continuation of horrible hack.
		Camera* mCamera;
        Map* mMap;
        ParticleSystem* mParticleSystem;
        Physics* mPhysics;
        InfastructureManager* mInfastructureManager;
        Minimap* mMinimap;
        
        InterfaceView* mInterfaceView;

#ifdef PROFILE
        DebugScreenView* mDebugView;
#endif

        Player* mPlayer;
        EnemyManager* mEnemyManager;
};
    
extern World* gWorld;

}

#endif
