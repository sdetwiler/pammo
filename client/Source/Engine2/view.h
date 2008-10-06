#ifndef __VIEW_H__
#define __VIEW_H__

#include "pammo.h"

namespace pammo
{

class Game;

enum ViewPriorities
{
    kImageLibraryPriority = 50,
    kMapPriority = 100,
    kPhysicsPriority = 101,
    kMainScreenPriority = 102,
    kCreditsScreenPriority = 103,
    kTutorialScreenPriority = 104,
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
    kShieldTogglePriority = 251,
    kHealthMeterPriority = 260,
    kEnergyMeterPriority = 261,
    kScoreMeterPriority = 262,
    kDeathCardPriority = 300

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
    
    void unregister();

    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    virtual void draw();
    
    void disableAll();
    void enableAll();
    void disableUpdate();
    void enableUpdate();
    void disableTouch();
    void enableTouch();
    void disableDraw();
    void enableDraw();

private:
    friend class Game;
    void init();

    enum Notifications
    {
        kDraw = 2,
        kTouch = 4,
        kUpdate = 8
    };

    uint32_t mNotifications;
};

}

#endif
