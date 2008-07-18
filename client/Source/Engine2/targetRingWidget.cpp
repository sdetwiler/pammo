#include "targetRingWidget.h"
#include "imageLibrary.h"
#include "image.h"

namespace pammo
{

TargetRingWidget::TargetRingWidget(uint32_t priority)
    : mPriority(priority),
    View()
{
    mObserver = 0;
    
    mCenter = Vector2(160, 70);
    mValue = Vector2(0, 0);
    mSize = 100;
    
    mRingImage = gImageLibrary->reference("data/interface/targetRingBackground.png");
    mIndicatorImage = gImageLibrary->reference("data/interface/targetRingSelection.png");
    
}

TargetRingWidget::~TargetRingWidget()
{
    if(mRingImage)
        gImageLibrary->unreference(mRingImage);
    if(mIndicatorImage)
        gImageLibrary->unreference(mIndicatorImage);
}
    
void TargetRingWidget::setObserver(TargetRingObserver* observer)
{
    mObserver = observer;
}

void TargetRingWidget::setCenter(Vector2 center)
{
    mCenter = center;
}

void TargetRingWidget::setSize(float size)
{
    mSize = size;
}

uint32_t TargetRingWidget::getTouchPriority() const
{
    return mPriority;
}

uint32_t TargetRingWidget::getUpdatePriority() const
{
    return mPriority;
}

uint32_t TargetRingWidget::getDrawPriority() const
{
    return mPriority;
}

bool TargetRingWidget::touch(uint32_t count, Touch* touches)
{
    for(uint32_t i=0; i < count; ++i)
    {
        Vector2 loc = touches[i].mLocation;
        
        Vector2 diff = loc - mCenter;
        float dist = magnitude(diff);
        
        if(dist > mSize/2 * 2) continue;
        else if(dist > mSize/2) diff = diff / dist * mSize / 2;
        
        mValue = diff / mSize * 2;
        
        if(mObserver)
            mObserver->onTargetRingUpdated(this, mValue);
    }
    
    return false;
}

void TargetRingWidget::update()
{
}

void TargetRingWidget::draw()
{
    // Create transform, cribbed from entity.cpp transform setup. Position about center, scale and rotate appropriately.
    // This probably should just use an imageEntity.
    float x = mSize;
    float y = mSize;
    Transform2 trans;
    trans *= Transform2::createTranslation(mCenter - Vector2(x/2, y/2));
	trans *= Transform2::createScale(Vector2(x, y));
    drawImage(mRingImage, trans, 100);
    
    float factor = 1/5.;
    
    trans = Transform2::createTranslation(mCenter - Vector2(x/2*factor, y/2*factor) + mValue*mSize/2);
	trans *= Transform2::createScale(Vector2(x*factor, y*factor));
    drawImage(mIndicatorImage, trans, 100);
}


} // namespace pamo