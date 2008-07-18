/*
 *  view.cpp
 *  pammo
 *
 *  Created by James Marr on 7/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "view.h"
#include "game.h"

namespace pammo
{

View::View()
{
    gGame->queueInit(this);
}

View::~View()
{
    if(getDrawPriority() != 0)
        gGame->unregisterDrawable(this);
    
    if(getUpdatePriority() != 0)
        gGame->unregisterUpdateable(this);
    
    if(getTouchPriority() != 0)
        gGame->unregisterTouchable(this);
}

void View::destroy()
{
    gGame->queueDelete(this);
}

void View::draw() {}

void View::update() {}

bool View::touch(uint32_t count, Touch* touches)
{
    return false;
}
    
uint32_t View::getDrawPriority() const
{
    return 0;
}

uint32_t View::getUpdatePriority() const
{
    return 0;
}

uint32_t View::getTouchPriority() const
{
    return 0;
}
    
void View::init()
{
    if(getDrawPriority() != 0)
        gGame->registerDrawable(this);
    
    if(getUpdatePriority() != 0)
        gGame->registerUpdateable(this);
    
    if(getTouchPriority() != 0)
        gGame->registerTouchable(this);
}

}
