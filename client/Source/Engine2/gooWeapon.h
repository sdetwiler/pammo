#ifndef __GOO_WEAPON_H__
#define __GOO_WEAPON_H__

#include "pammo.h"

#include "weapon.h"

namespace pammo
{

class GooWeapon : public Weapon
{
    public:
        GooWeapon();
        virtual ~GooWeapon();
        
        virtual void select();
        virtual void deselect();
        
        virtual void fire();
};

}

#endif