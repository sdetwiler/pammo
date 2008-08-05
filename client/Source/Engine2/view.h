/*
 *  view.h
 *  pammo
 *
 *  Created by James Marr on 7/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __VIEW_H__
#define __VIEW_H__

#include "pammo.h"

namespace pammo
{

class Game;

enum ViewPriorities
{
    kMapPriority = 100,
    kParticle0Priority = 120,
    kParticle1Priority = 125,
    kEnemyManagerPriority = 140,
    kPlayerPriority = 150,
    kParticle2Priority = 155,
    kParticle3Priority = 156,
    kMoveRingPriority = 200,
    kFireRingPriority = 201,
    kWeaponSelectorPriority = 250
};

class View
{
public:
    View();
    virtual ~View();
    void destroy();
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;
    
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    virtual void draw();
    
private:
    friend class Game;
    void init();
};

}

#endif