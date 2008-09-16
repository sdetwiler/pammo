#include "targetRingWidget.h"
#include "imageLibrary.h"
#include "drawImage.h"

namespace pammo
{

TargetRingWidget::TargetRingWidget(uint32_t priority, Image* image)
    : mPriority(priority),
    View()
{
    mObserver = 0;
    mImage.setImage(image);
    
    reset();
}

TargetRingWidget::~TargetRingWidget()
{}

void TargetRingWidget::reset()
{
    mValue = 0;
    mImage.mRotation = mValue;
    mImage.makeDirty();
    
    mTrackingSerialNumber = 0;
}
    
void TargetRingWidget::setObserver(TargetRingObserver* observer)
{
    mObserver = observer;
}
    
TargetRingObserver* TargetRingWidget::getObserver()
{
    return mObserver;
}

void TargetRingWidget::setCenter(Vector2 center)
{
    mImage.mCenter = center;
    mImage.makeDirty();
}

Vector2 TargetRingWidget::getCenter()
{
    return mImage.mCenter;
}

void TargetRingWidget::setSize(Vector2 size)
{
    mImage.mSize = size;
    mImage.makeDirty();
}

Vector2 TargetRingWidget::getSize()
{
    return mImage.mSize;
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
        // If this is the end of a touch.
        if(touches[i].mPhase == Touch::PhaseEnd)
        {
            // If this is not the touch we are tracking, continue.
            if(touches[i].mSerialNumber != mTrackingSerialNumber)
                continue;
            
            // Stop tracking.
            mTrackingSerialNumber = 0;
            if(mObserver)
                mObserver->onTargetRingUntouched(this);
            
            return true;
        }
        
        // A touch or a move, we don't care about. If we are tracking a different touch, bail.
        if(mTrackingSerialNumber != 0 && mTrackingSerialNumber != touches[i].mSerialNumber)
            continue;
        
        // Calculate the distance from the touch to the center of this widget.
        Vector2 diff = touches[i].mLocation - mImage.mCenter;
        float dist = magnitude(diff);
        
        // If its larger than the widget, bail.
        if(dist > mImage.mSize.x/2)
            continue;
            
        // Otherwise, this is a valid touch. Remember the serial. Update the image. Call observer.
        mTrackingSerialNumber = touches[i].mSerialNumber;
        mValue = atan2(diff.y, diff.x);
        mImage.mRotation = mValue + M_PI/2;
        mImage.makeDirty();
        if(mObserver)
            mObserver->onTargetRingTouched(this, mValue);
        
        return true;
    }
    
    return false;
}

void TargetRingWidget::update()
{
}

void TargetRingWidget::draw()
{
    mImage.draw();
}


} // namespace pamo