#ifndef __TARGET_RING_WIDGET_H__
#define __TARGET_RING_WIDGET_H__

#include "pammo.h"
#include "view.h"
#include "imageEntity.h"

namespace pammo
{

class TargetRingWidget;

class TargetRingObserver
{
    public:
        virtual void onTargetRingTouched(TargetRingWidget *widget, float value) = 0;
        virtual void onTargetRingUntouched(TargetRingWidget* widget) = 0;
};

class TargetRingWidget : public View
{
public:
    TargetRingWidget(uint32_t priority, Image* image);
    virtual ~TargetRingWidget();
    
    void reset();
    
    void setObserver(TargetRingObserver* observer);
    TargetRingObserver* getObserver();
    
    void setCenter(Vector2 center);
    Vector2 getCenter();
    
    void setSize(Vector2 size);
    Vector2 getSize();

    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;
		
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    virtual void draw();

private:
    uint32_t mPriority;
    TargetRingObserver* mObserver;
    ImageEntity mImage;
    
    float mValue;
    void* mTrackingSerialNumber;
};

} // namespace pammo

#endif