#ifndef __FLAMETHROWER_WEAPON_H__
#define __FLAMETHROWER_WEAPON_H__

#include "pammo.h"

#include "weapon.h"

namespace pammo
{

class FlamethrowerWeapon : public Weapon
{
    public:
        FlamethrowerWeapon();
        virtual ~FlamethrowerWeapon();
        
        virtual void select();
        virtual void deselect();
        
        virtual void fire();
};
}

#endif