#ifndef __WEAPON_SELECTOR_H__
#define __WEAPON_SELECTOR_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

class Weapon;
class WeaponSelector;
class ImageEntity;

class WeaponSelectorObserver
{
    public:
        virtual void onWeaponSelectorUpdated(WeaponSelector* widget, Weapon* weapon) = 0;
};

class WeaponSelector : public View
{
    public:
        WeaponSelector();
        virtual ~WeaponSelector();
    
        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
    
        void reset();
        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();
        virtual void draw();
        
        void setObserver(WeaponSelectorObserver* observer);
        
        void nextWeapon();
        
        void setSelected(Weapon* weapon);
        Weapon* getSelected();
        
        void addWeapon(Weapon* weapon);
    
    private:
        WeaponSelectorObserver* mObserver;
        
        typedef vector< Weapon* > WeaponVector;
        WeaponVector mWeapons;
        Weapon* mSelected;
        
        ImageEntity* mHighlight;
};

}

#endif