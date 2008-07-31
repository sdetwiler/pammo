#ifndef __WEAPON_SELECTOR_H__
#define __WEAPON_SELECTOR_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

class Weapon;
class ImageEntity;

class WeaponSelector : public View
{
    public:
        WeaponSelector();
        virtual ~WeaponSelector();
    
        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getDrawPriority() const;
    
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void draw();
        
        void addWeapon(Weapon* weapon);
    
    private:
        typedef vector< Weapon* > WeaponVector;
        WeaponVector mWeapons;
        Weapon* mSelected;
        
        ImageEntity* mHighlight;
};

}

#endif