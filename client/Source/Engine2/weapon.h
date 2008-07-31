#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "pammo.h"

#include "imageEntity.h"

namespace pammo
{

class Weapon
{
    public:
        Weapon();
        virtual ~Weapon() = 0;
        
        ImageEntity* getIcon();
        
        virtual void select() = 0;
        virtual void deselect() = 0;
        
        virtual void fire() = 0;
    
    protected:
        ImageEntity mIcon;
};
    
}

#endif
