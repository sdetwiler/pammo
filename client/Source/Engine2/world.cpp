#include "world.h"

#include "levelLoader.h"
#include "builder.h"

#include "game.h"
#include "camera.h"
#include "imageLibrary.h"
#include "player.h"
#include "infastructureManager.h"
#include "map.h"
#include "particleSystem.h"
#include "physics.h"
#include "minimap.h"
#include "enemyManager.h"
#include "enemyLoader.h"
#include "interfaceView.h"
#include "mainScreenView.h"

#ifdef PROFILE
#include "debugScreenView.h"
#endif 

namespace pammo
{
    
World* gWorld = NULL;

World::World()
{
    gWorld = this;

    mBackgroundAudio = gAudioLibrary->getAudioInstance(AUDIO_BACKGROUND_INGAME);

    mCamera = new Camera();
    mMap = new Map();
    
    mParticleSystem = new ParticleSystem(1600);
    
    mPhysics = new Physics(800);
    
    mMinimap = new Minimap();
    
    mEnemyManager = new EnemyManager;

    mInterfaceView = new InterfaceView;

#ifdef PROFILE
    //mDebugView = new DebugScreenView;
#endif

    mPlayer = new Player();
    
    mInfastructureManager = new InfastructureManager();

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

#ifdef PROFILE
    mDebugView->destroy();
#endif

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

InfastructureManager* World::getInfastructureManager() const
{
    return mInfastructureManager;
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
    mInfastructureManager->reset();
}

void World::disable()
{
    gAudioLibrary->stopAudioInstance(mBackgroundAudio);

    mPlayer->disable();
    mParticleSystem->disable();

    mInfastructureManager->disableAll();
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

    mInfastructureManager->enableAll();
    mMap->enableAll();
    mPhysics->enableAll();
    mMinimap->enableAll();
    mInterfaceView->enableAll();
    mEnemyManager->enableAll();
    gAudioLibrary->playAudioInstance(mBackgroundAudio, false);

}

} //namespace pammo
