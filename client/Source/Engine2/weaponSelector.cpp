#include "weaponSelector.h"

#include "weapon.h"
#include "world.h"
#include "player.h"
#include "imageEntity.h"
#include "imageLibrary.h"

namespace pammo
{

const float kIconSize = 32;
//const Vector2 kIconBorder = Vector2(0, 8);

WeaponSelector::WeaponSelector()
{
    mObserver = 0;
    
    mSelected = 0;
    
    mHighlight = new ImageEntity(gImageLibrary->reference("data/interface/highlightWeaponIcon.png"));
    mHighlight->mSize = Vector2(kIconSize, kIconSize);
    mHighlight->mRotation = 0;
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

void WeaponSelector::reset()
{
    setSelected(0);
    
    mWeapons.clear();
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
            
            setSelected(*i);
            return true;
        }
    }
    return false;
}

void WeaponSelector::draw()
{
    glLoadIdentity();
    mHighlight->mCenter = mSelected->getIcon()->mCenter;
    mHighlight->makeDirty();
    mHighlight->draw();
    
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        (*i)->getIcon()->draw();
    }
}

void WeaponSelector::setObserver(WeaponSelectorObserver* observer)
{
    mObserver = observer;
}

void WeaponSelector::setSelected(Weapon* weapon)
{
    if(mSelected)
        mSelected->deselect();
    
    mSelected = weapon;
    
    if(mSelected)
        mSelected->select();
    
    if(mObserver)
        mObserver->onWeaponSelectorUpdated(this, mSelected);
}

void WeaponSelector::addWeapon(Weapon* weapon)
{
    const Vector2 kIconBorder = Vector2(0, 8);

    mWeapons.push_back(weapon);
    
    // Update the icon size.
    ImageEntity* entity = weapon->getIcon();
    entity->mSize = Vector2(kIconSize, kIconSize);
    entity->makeDirty();
    
    // Iterate over each icon and set the position accordingly.
    Vector2 frame = getFrameSize();
    float y = frame.y - 240.0f + ((kIconSize + kIconBorder.y)/2.0f);
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        entity = (*i)->getIcon();
        entity->mCenter = Vector2(frame.x - (kIconSize/2.0f) - kIconBorder.x, y);
        entity->makeDirty();
        
        y -= (kIconSize + kIconBorder.y);
    }
    
    // If this is the first weapon, set it on the player.
    if(!mSelected)
        setSelected(weapon);
}

}