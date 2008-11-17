#ifndef __LIGHTNING_WEAPON_H__
#define __LIGHTNING_WEAPON_H__

#include "pammo.h"

#include "weapon.h"

namespace pammo
{

class LightningWeapon : public Weapon
{
    public:
        LightningWeapon();
        virtual ~LightningWeapon();
        
        virtual void select();
        virtual void deselect();
        
        virtual void fire();
    
        uint32_t mParticleCount;
        AudioInstance* mAudio;
};

}

#endif