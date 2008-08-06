#include "world.h"
#include "levelLoader.h"
#include "game.h"
#include "camera.h"
#include "imageLibrary.h"
#include "player.h"
#include "map.h"
#include "builder.h"
#include "particleSystem.h"
#include "physics.h"
#include "enemyManager.h"

namespace pammo
{
    
World* gWorld = NULL;

World::World()
{
    gWorld = this;

    mCamera = new Camera();
    mMap = new Map();
    
    mParticleSystem = new ParticleSystem(500);
    
    mPhysics = new Physics();
    
    mEnemyManager = new EnemyManager;


    mPlayer = new Player();
    
    char mapName[256];
    LevelLoader loader;
    if(loader.load("data/levels/1.csv", mapName) == false)
    {
        dprintf("Failed to load level definition file.");
        assert(0);
    }
    buildFromMap(this, mapName);
}

World::~World()
{
    mPlayer->destroy();
    mPhysics->destroy();
    //mParticleSystem->destroy();
    delete mParticleSystem;
    delete mCamera;
    delete mMap;
    
    gWorld = NULL;
}

Camera* World::getCamera()
{
    return mCamera;
}

Map* World::getMap()
{
    return mMap;
}

ParticleSystem* World::getParticleSystem()
{
    return mParticleSystem;
}

Physics* World::getPhysics()
{
    return mPhysics;
}

Player* World::getPlayer() const
{
    return mPlayer;
}

EnemyManager* World::getEnemyManager() const
{
    return mEnemyManager;
}

} //namespace pammo
