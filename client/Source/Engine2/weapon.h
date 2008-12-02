#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "pammo.h"

#include "imageEntity.h"

namespace pammo
{

enum WeaponDisplayPriority
{
    kLightningWeaponDisplayPriority = 1,
    kGrenadeLauncherWeaponDisplayPriority,
    kGooWeaponDisplayPriority
};

class Weapon
{
    public:
        Weapon();
        virtual ~Weapon() = 0;
        
        ImageEntity* getIcon();
        
        virtual void select() = 0;
        virtual void deselect() = 0;
        
        virtual void fire() = 0;
        virtual uint32_t getDisplayPriority() = 0;
        
        Vector2 getTargetLocation() { return mTargetLocation; }
        void setTargetLocation(Vector2 targetLocation) { mTargetLocation = targetLocation; }
    
    protected:
        ImageEntity mIcon;
        Vector2 mTargetLocation;
};
    
}

#endif
