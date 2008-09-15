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
#include "minimap.h"
#include "enemyManager.h"
#include "enemyLoader.h"
#include "interfaceView.h"
#include "mainScreenView.h"

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
    
    mMinimap = new Minimap();
    
    mEnemyManager = new EnemyManager;

    mInterfaceView = new InterfaceView;

    mPlayer = new Player();

    char mapName[256];
    LevelLoader loader;
    if(loader.load("data/levels/1.csv", mapName) == false)
    {
        dprintf("Failed to load level definition file.");
        assert(0);
    }
    buildFromMap(this, mapName);
    mPlayer->setCenter(mPlayer->getSpawnPoint());
}

World::~World()
{
    mParticleSystem->reset();
    mParticleSystem->destroy();

    mMap->destroy();
    mEnemyManager->destroy();
    
    mPlayer->destroy();
    mPhysics->destroy();
    mMinimap->destroy();
    mInterfaceView->destroy();

    delete mParticleSystem;
    delete mCamera;
    
    gWorld = NULL;

    new MainScreenView;
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

Minimap* World::getMinimap()
{
    return mMinimap;
}

Player* World::getPlayer() const
{
    return mPlayer;
}

EnemyManager* World::getEnemyManager() const
{
    return mEnemyManager;
}

void World::reset()
{
    mParticleSystem->reset();
    mPlayer->reset();
    mEnemyManager->reset();
 //   mPhysics->reset();
}

void World::disable()
{
    mPlayer->disable();
    mParticleSystem->disable();

    mMap->disableAll();
    mPhysics->disableAll();
    mMinimap->disableAll();
    mInterfaceView->disableAll();
    mEnemyManager->disableAll();
}

void World::enable()
{
    mPlayer->enable();
    mParticleSystem->enable();

    mMap->enableAll();
    mPhysics->enableAll();
    mMinimap->enableAll();
    mInterfaceView->enableAll();
    mEnemyManager->enableAll();

}

} //namespace pammo
