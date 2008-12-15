#ifndef __INFASTRUCTURE_MANAGER_H__
#define __INFASTRUCTURE_MANAGER_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

enum PowerupType
{
    kPowerupNone                  = 0,
    
    kPowerupShield                = 1<<0,
    kPowerupGooWeapon             = 1<<1,
    kPowerupGrenadeLauncherWeapon = 1<<2,
    
    kPowerupLifeUpgrade           = 1<<10,
    kPowerupEnergyUpgrade         = 1<<11,
    
    kPowerupLifeRestore           = 1<<20,
    kPowerupEnergyRestore         = 1<<21,
};

class InfastructureManager : public View
{
    public:
        InfastructureManager();
        void reset();

        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
        
        virtual void update();
        virtual void draw();
        
        void setLocation(Vector2 location);
        Vector2 getLocation();
        
    private:
        Vector2 mLocation;
        uint32_t mGivenNewPowers;
        uint32_t mNextNewPowerScore;
        float mNextNewPowerMultiplier;
        uint32_t mNextUpgradeScore;
        float mNextUpgradeMultiplier;
        uint32_t mNextRestoreScore;
        float mNextRestoreMultiplier;
        
        float mUpgradeScale;
    
        bool createPowerup(PowerupType type);
};

}

#endif