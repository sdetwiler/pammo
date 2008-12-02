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
    
    mHighlight = new ImageEntity(gImageLibrary->getImage(INTERFACE_ICON_WEAPON_HIGHLIGHT));
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

uint32_t WeaponSelector::getUpdatePriority() const
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

void WeaponSelector::update()
{
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        Weapon* weapon = *i;
        ImageEntity* e = weapon->getIcon();
        if(e->mAlpha < 1)
        {
            e->mAlpha += 0.02;
            if(e->mAlpha > 1)
                e->mAlpha = 1;
        }
        
        const float moveRate = 1.5;
        Vector2 target = weapon->getTargetLocation();
        if(e->mCenter.x > target.x)
        {
            e->mCenter.x -= moveRate;
            if(e->mCenter.x < target.x)
                e->mCenter.x = target.x;
            e->makeDirty();
        }
        else if(e->mCenter.x < target.x)
        {
            e->mCenter.x += moveRate;
            if(e->mCenter.x > target.x)
                e->mCenter.x = target.x;
            e->makeDirty();
        }
        if(e->mCenter.y > target.y)
        {
            e->mCenter.y -= moveRate;
            if(e->mCenter.y < target.y)
                e->mCenter.y = target.y;
            e->makeDirty();
        }
        else if(e->mCenter.y < target.y)
        {
            e->mCenter.y += moveRate;
            if(e->mCenter.y > target.y)
                e->mCenter.y = target.y;
            e->makeDirty();
        }
    }
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

void WeaponSelector::nextWeapon()
{
    // Find index of current weapon.
    int32_t index = 0;
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        if((*i) == mSelected) break;
        ++index;
    }
    
    // Select the next weapon.
    index = (index+1) % mWeapons.size();
    setSelected(mWeapons[index]);
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
    const Vector2 kIconBorder = Vector2(8, 16);

    mWeapons.push_back(weapon);
    
    // Iteratively sort weapons. Iterate backwards, bubbling up.
    for(uint32_t i=mWeapons.size()-1; i > 0; --i)
    {
        if(mWeapons[i]->getDisplayPriority() < mWeapons[i-1]->getDisplayPriority())
        {
            Weapon* tmp = mWeapons[i];
            mWeapons[i] = mWeapons[i-1];
            mWeapons[i-1] = tmp;
        }
    }
    
    // Update the icon size.
    ImageEntity* entity = weapon->getIcon();
    entity->mSize = Vector2(kIconSize, kIconSize);
    entity->makeDirty();
    
    // Iterate over each icon and set the position accordingly.
    Vector2 frame = getFrameSize();
    float y = frame.y - 194.0f + ((kIconSize + kIconBorder.y)/2.0f);
    for(WeaponVector::iterator i=mWeapons.begin(); i != mWeapons.end(); ++i)
    {
        (*i)->setTargetLocation(Vector2(frame.x - (kIconSize/2.0f) - kIconBorder.x, y));
        y -= (kIconSize + kIconBorder.y);
    }
    
    if(mWeapons.size() == 1)
    {
        // One icon, select bar is hidden. Set values so update function does nothing.
        weapon->getIcon()->mAlpha = 1;
        weapon->getIcon()->mCenter = weapon->getTargetLocation();
        weapon->getIcon()->makeDirty();
    }
    else if(mWeapons.size() == 2)
    {
        mWeapons[0]->getIcon()->mAlpha = 0;
        mWeapons[0]->getIcon()->mCenter = mWeapons[0]->getTargetLocation();
        mWeapons[0]->getIcon()->makeDirty();
        
        mWeapons[1]->getIcon()->mAlpha = 0;
        mWeapons[1]->getIcon()->mCenter = mWeapons[1]->getTargetLocation();
        mWeapons[1]->getIcon()->makeDirty();
    }
    else if(mWeapons.size() == 3)
    {
        weapon->getIcon()->mAlpha = 0;
        weapon->getIcon()->mCenter = weapon->getTargetLocation();
        weapon->getIcon()->makeDirty();
    }
    
    // If this is the first weapon, set it on the player.
    if(!mSelected)
        setSelected(weapon);
}

}