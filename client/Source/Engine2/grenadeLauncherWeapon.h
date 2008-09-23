#ifndef __GRENADELAUNCHER_WEAPON_H__
#define __GRENADELAUNCHER_WEAPON_H__

#include "pammo.h"

#include "weapon.h"

namespace pammo
{

class GrenadeLauncherWeapon : public Weapon
{
    public:
        GrenadeLauncherWeapon();
        virtual ~GrenadeLauncherWeapon();
        
        virtual void select();
        virtual void deselect();
        
        virtual void fire();
        uint64_t mLastFire;
        uint64_t mFireFrequency;
};

}

#endif