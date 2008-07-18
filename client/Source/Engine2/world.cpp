/*
 *  world.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "world.h"
#include "game.h"
#include "camera.h"
#include "imageLibrary.h"
#include "player.h"
#include "map.h"
#include "builder.h"
#include "particleSystem.h"

namespace pammo
{
    
World* gWorld = NULL;

World::World()
{
    gWorld = this;

    mCamera = new Camera(Vector2(0, 0), getFrameSize());
    mMap = new Map();
    
    mPlayer = new Player();
    
    mParticleSystem = new ParticleSystem(500);
    
    buildFromMap(this, "TheMap");
}

World::~World()
{
    mPlayer->destroy();
    
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

} //namespace pammo
