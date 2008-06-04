/*
 *  lobbyView.cpp
 *  pammo
 *
 *  Created by James Marr on 6/3/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "lobbyView.h"
#include "world.h"

LobbyView::LobbyView()
  : Widget()
{
    setTouchPriority(200);
    setDrawPriority(200);
    
    gGame->registerTouchable(this);
    gGame->registerDrawable(this);
    gGame->registerUpdateable(this);
    
    createWorld();
}

LobbyView::~LobbyView()
{
    gGame->unregisterTouchable(this);
    gGame->unregisterDrawable(this);
    gGame->unregisterUpdateable(this);
}

void LobbyView::draw()
{}

bool LobbyView::touch(uint32_t count, Touch* touches)
{
    dprintf("Loading game");
    
    if(gWorld) return false;
    
    createWorld();
    
    return true;
}

void LobbyView::update(int delta)
{}

void LobbyView::createWorld()
{
    // Now create the world.
    gWorld = new World();
    
    // Initialize the world.
    assert(gWorld->init("Desert") == 0);
}

