#include "weaponSelector.h"

#include "weapon.h"
#include "world.h"
#include "player.h"
#include "imageEntity.h"
#include "imageLibrary.h"

namespace pammo
{

const float kIconSize = 32;
const Vector2 kIconBorder = Vector2(24, 8);

WeaponSelector::WeaponSelector()
{
    mHighlight = new ImageEntity(gImageLibrary->reference("data/interface/highlightWeaponIcon.png"));
    mHighlight->mSize = Vector2(kIconSize, kIconSize);
    mHighlight->makeDirty();
}

WeaponSelector::~WeaponSelector()
{
}

uint32_t WeaponSelector::getTouchPriority() const
{
    return kWeaponSelectorPriority;
}

uint32_t WeaponSelector::getDrawPriority() const
{
    return kWeaponSelectorPriority;
}

bool WeaponSelector::touch(uint32_t count, Touch* touches)
{
    for(uint32_t i=0; i < count; ++i)
    {
        if(touches[i].mPhase != Touch::PhaseEnd)
            continue;
            
        Vector2 loc = touches[i].mLocation;
        
        for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
        {
            ImageEntity* e = (*i)->getIcon();
            
            if(e->mCenter.x - e->mSize.x > loc.x
                || e->mCenter.x + e->mSize.x < loc.x)
                    continue;
                    
            if(e->mCenter.y - e->mSize.y > loc.y
                || e->mCenter.y + e->mSize.y < loc.y)
                    continue;
            
            mSelected = *i;
            gWorld->getPlayer()->mWeapon = *i;
            return true;
        }
    }
    return false;
}

void WeaponSelector::draw()
{
    mHighlight->mCenter = mSelected->getIcon()->mCenter;
    mHighlight->makeDirty();
    mHighlight->draw();
    
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        (*i)->getIcon()->draw();
    }
}

void WeaponSelector::addWeapon(Weapon* weapon)
{
    mWeapons.push_back(weapon);
    
    // Update the icon size.
    ImageEntity* entity = weapon->getIcon();
    entity->mSize = Vector2(kIconSize, kIconSize);
    entity->makeDirty();
    
    // Iterate over each icon and set the position accordingly.
    Vector2 frame = getFrameSize();
    float x = frame.x/2 - (kIconSize + kIconBorder.x)/2 * (mWeapons.size()-1);
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        entity = (*i)->getIcon();
        entity->mCenter = Vector2(x, frame.y - kIconSize/2 - kIconBorder.y);
        entity->makeDirty();
        
        x += kIconSize + kIconBorder.x;
    }
    
    // If this is the first weapon, set it on the player.
    if(mWeapons.size() == 1)
    {
        mSelected = weapon;
        gWorld->getPlayer()->mWeapon = weapon;
    }
}

}