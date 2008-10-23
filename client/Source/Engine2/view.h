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
    kInfastructureManagerPriority = 145,
    kEnemyManagerPriority = 150,
    kParticle2Priority = 155,
    kParticle3Priority = 156,
    
    kDebugScreenPriority = 170,

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

#ifndef PROFILE

class ViewProfiler
{
public:
    ViewProfiler() {}
    void setName(char const* name) { }
    void reset() { }
    void startRound() { }
    void endRound() { }
    void start(uint32_t prio) { }
    void end(uint32_t prio) { }
    void report() { }
};

#else

class ViewProfiler
{
public:
    ViewProfiler()
    {
        mName[0] = 0;
        reset();
    }
    virtual ~ViewProfiler()
    {}
    
    void setName(char const* name)
    {
        strcpy(mName, name);
    }
    void reset()
    {
        for(uint32_t i=0; i<kDeathCardPriority; ++i)
        {
            mTime[i] = 0;
        }
        mRounds = 0;
        mRoundTime = 0;
    }

    void startRound()
    {
        mRoundStart = getTime();
    }

    void endRound()
    {
        mRoundTime += (uint32_t)(getTime() - mRoundStart);
        ++mRounds;

        if(mRounds == 60)
        {
            report();
            reset();
        }
    }

    void start(uint32_t prio)
    {
        mStart = getTime();
    }

    void end(uint32_t prio)
    {
        mTime[prio]+= (uint32_t)(getTime() - mStart);
    }

    void report()
    {
        uint32_t aveRoundTime = mRoundTime/mRounds;
        dprintf("%s Ave: %u", mName, aveRoundTime);
        
        uint32_t timeSpent = 0;
        
        for(uint32_t i=0; i<kDeathCardPriority; ++i)
        {
            if(mTime[i] != 0)
            {
                timeSpent+= mTime[i];
                uint32_t aveTime = mTime[i]/mRounds;
                float pct = ((float)mTime[i] / (float)mRoundTime)*100.0f;
                dprintf("  % 6u ave: % 6u  pct: % 2.2f%%", i, aveTime, pct);
            }
        }
        
        dprintf("%s round: % 6u time: % 6u overhead: % 6u (%.2f%%)\n\n", 
            mName, 
            (uint32_t)mRoundTime, 
            timeSpent, 
            mRoundTime-timeSpent, 
            ((float)(mRoundTime-timeSpent)/(float)mRoundTime)*100.0f);
    }

protected:

private:

    char     mName[256];
    uint64_t mStart;
    uint32_t mRounds;
    uint32_t mTime[kDeathCardPriority];
    uint64_t mRoundStart;
    uint32_t mRoundTime;

};

#endif

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
    virtual void lowMemory();
    
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
