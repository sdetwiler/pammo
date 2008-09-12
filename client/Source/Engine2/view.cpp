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
    mNotifications = kDraw | kTouch | kUpdate;
    gGame->queueInit(this);
}

View::~View()
{
}

void View::unregister()
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
    disableAll();
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

void View::disableAll()
{
    mNotifications = 0;
}

void View::enableAll()
{
    if(getDrawPriority() != 0)
        enableDraw();
    
    if(getUpdatePriority() != 0)
        enableUpdate();
    
    if(getTouchPriority() != 0)
        enableTouch();
}

void View::disableUpdate()
{
    mNotifications = mNotifications & ~kUpdate;
}

void View::enableUpdate()
{
    mNotifications = mNotifications | kUpdate;
}

void View::disableTouch()
{
    mNotifications = mNotifications & ~kTouch;
}

void View::enableTouch()
{
    mNotifications = mNotifications | kTouch;
}

void View::disableDraw()
{
    mNotifications = mNotifications & ~kDraw;
}

void View::enableDraw()
{
    mNotifications = mNotifications | kDraw;
}


}
