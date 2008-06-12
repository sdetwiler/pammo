#ifndef __TARGETRINGWIDGET_H__
#define __TARGETRINGWIDGET_H__

#include "widget.h"
#include "vehicle.h"

namespace pammo
{

class TargetRingWidget 
{
public:
    TargetRingWidget(Vehicle* vehicle);
    virtual ~TargetRingWidget();

    int init();

    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);

protected:

private:
    bool mHighlighted;
    Vehicle* mVehicle;
    Image* mImage;
    Image* mHighlightImage;
    float mAngle;
};

} // namespace pammo

#endif