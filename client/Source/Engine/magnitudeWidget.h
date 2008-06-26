#ifndef __MAGNITUDEWIDGET_H__
#define __MAGNITUDEWIDGET_H__

#include "widget.h"
#include "vehicle.h"

namespace pammo
{

class MagnitudeWidget 
{
public:
    MagnitudeWidget(Vehicle* vehicle);
    virtual ~MagnitudeWidget();

    int init();

    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);

protected:

private:
///    bool mHighlighted;
    Vehicle* mVehicle;
    Image* mImage;
//    Image* mHighlightImage;
    float mAngle;
    float mMagnitude;
    bool  mTracking;
};

} // namespace pammo

#endif