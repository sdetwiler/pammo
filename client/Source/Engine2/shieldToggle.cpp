#include "shieldToggle.h"

#include "world.h"
#include "player.h"
#include "imageEntity.h"
#include "imageLibrary.h"

namespace pammo
{

const float kIconSize = 32;
//const Vector2 kIconBorder = Vector2(0, 8);

ShieldToggle::ShieldToggle()
{
    mObserver = 0;
    mToggle = 0;
    
    mDisabledImage = gImageLibrary->reference("data/interface/shieldEnableIcon.png");
    mEnabledImage = gImageLibrary->reference("data/interface/shieldDisableIcon.png");
    
    mEntity = new ImageEntity();
    mEntity->mCenter = Vector2(20, 100);
    mEntity->mSize = Vector2(kIconSize, kIconSize);
    mEntity->makeDirty();
}

ShieldToggle::~ShieldToggle()
{
}

uint32_t ShieldToggle::getTouchPriority() const
{
    return kShieldTogglePriority;
}

uint32_t ShieldToggle::getDrawPriority() const
{
    return kShieldTogglePriority;
}

void ShieldToggle::reset()
{
    setToggle(false);
}

bool ShieldToggle::touch(uint32_t count, Touch* touches)
{
    for(uint32_t i=0; i < count; ++i)
    {
        if(touches[i].mPhase != Touch::PhaseEnd)
            continue;
            
        Vector2 loc = touches[i].mLocation;
        
        if(mEntity->mCenter.x - mEntity->mSize.x > loc.x
            || mEntity->mCenter.x + mEntity->mSize.x < loc.x)
                continue;
                
        if(mEntity->mCenter.y - mEntity->mSize.y > loc.y
            || mEntity->mCenter.y + mEntity->mSize.y < loc.y)
                continue;
        
        setToggle(!mToggle);
        return true;
    }
    return false;
}

void ShieldToggle::draw()
{
    glLoadIdentity();
    
    mEntity->draw();
}

void ShieldToggle::setObserver(ShieldToggleObserver* observer)
{
    mObserver = observer;
}

void ShieldToggle::setToggle(bool toggle)
{
    mToggle = toggle;
    
    if(mToggle)
        mEntity->setImageWithoutSize(mEnabledImage);
    else
        mEntity->setImageWithoutSize(mDisabledImage);
    
    mObserver->onShieldToggleUpdated(this, mToggle);
}

bool ShieldToggle::getToggle()
{
    return mToggle;
}

}