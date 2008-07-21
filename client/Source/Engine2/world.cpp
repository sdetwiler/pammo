#include "world.h"
#include "game.h"
#include "camera.h"
#include "imageLibrary.h"
#include "player.h"
#include "map.h"
#include "builder.h"
#include "particleSystem.h"
#include "physics.h"

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
    
    mPlayer = new Player();
    
    buildFromMap(this, "TheMap");
}

World::~World()
{
    mPlayer->destroy();
    mPhysics->destroy();
    mParticleSystem->destroy();
    
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

} //namespace pammo
