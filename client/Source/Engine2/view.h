#ifndef __VIEW_H__
#define __VIEW_H__

#include "pammo.h"

namespace pammo
{

class Game;

enum ViewPriorities
{
    kMapPriority = 100,
    kPhysicsPriority = 101,
    kScreenPriority = 102,
    kParticle0Priority = 120,
    kParticle1Priority = 125,
    kPlayerPriority = 140,
    kEnemyManagerPriority = 150,
    kParticle2Priority = 155,
    kParticle3Priority = 156,
    kInterfaceViewPriority = 190,
    kMoveRingPriority = 200,
    kFireRingPriority = 201,
    kMinimapPriority = 230,
    kWeaponSelectorPriority = 250,
    kHealthMeterPriority = 260,
    kScoreMeterPriority = 261
};

class View
{
public:
    View();
    virtual ~View();
    virtual void destroy();
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;
    
    void unregister();

    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    virtual void draw();
    
private:
    friend class Game;
    void init();
};

}

#endif
