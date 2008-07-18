#ifndef __TARGET_RING_WIDGET_H__
#define __TARGET_RING_WIDGET_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class TargetRingWidget;
class Image;

class TargetRingObserver
{
    public:
        virtual void onTargetRingUpdated(TargetRingWidget *widget, Vector2 value) = 0;
};

class TargetRingWidget : public View
{
public:
    TargetRingWidget(uint32_t priority);
    virtual ~TargetRingWidget();
    
    void setObserver(TargetRingObserver* observer);
    void setCenter(Vector2 center);
    void setSize(float size);
    
    Vector2 getValue();

    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;
		
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    virtual void draw();

private:
    uint32_t mPriority;
    TargetRingObserver* mObserver;
    Vector2 mCenter, mValue;
    float mSize;
    Image* mRingImage;
    Image* mIndicatorImage;
};

} // namespace pammo

#endif