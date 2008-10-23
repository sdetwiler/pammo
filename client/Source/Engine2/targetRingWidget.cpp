#include "targetRingWidget.h"
#include "imageLibrary.h"

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
    
    mLastTouch = 0;
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
    assert(count == 1);
    for(uint32_t i=0; i < count; ++i)
    {
        // Capture the current time, used for interval tracking on double click.    
        uint64_t currentTime = getTime();
        
        //dprintf("Touch - Phase: %d Serial: %d Tracking: %d Count: %d Last: %d Cur: %d Delta: %d", touches[i].mPhase, touches[i].mSerialNumber, mTrackingSerialNumber, mTouchCount, (uint32_t)mLastTouch, (uint32_t)currentTime, (uint32_t)(currentTime - mLastTouch));
        
        // If this is the end of a touch.
        if(touches[i].mPhase == Touch::PhaseEnd)
        {
            // If this is not the touch we are tracking, continue.
            if(touches[i].mSerialNumber != mTrackingSerialNumber)
            {
                //dprintf("Distraction Untouch");
                continue;
            }
            
            // Stop tracking.
            mTrackingSerialNumber = 0;
            if(mObserver)
                mObserver->onTargetRingUntouched(this);
            
            //dprintf("Untouched");
            return true;
        }
        
        // A touch or a move, we don't care about. If we are tracking a different touch, bail.
        if(mTrackingSerialNumber != 0 && mTrackingSerialNumber != touches[i].mSerialNumber)
        {
            //dprintf("Distraction");
            continue;
        }
        
        // Calculate the distance from the touch to the center of this widget.
        Vector2 diff = touches[i].mLocation - mImage.mCenter;
        float dist = magnitude(diff);
        
        // If its larger than the widget, bail.
        if(dist > mImage.mSize.x/2)
        {
            //dprintf("Outside of range");
            continue;
        }
        
        // If this is starting a new touch, calculate time from last touch.
        if(mTrackingSerialNumber == 0)
        {
            if(currentTime - mLastTouch < 250000)
            {
                if(mObserver)
                    mObserver->onTargetRingDoubleTouched(this);
            }
            //else
            //{
            //    dprintf("Restouch too late");
            //}
            mLastTouch = currentTime;
        }
            
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