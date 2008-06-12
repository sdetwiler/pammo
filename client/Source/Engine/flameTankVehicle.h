#ifndef __FLAMETANKVEHICLE_H__
#define __FLAMETANKVEHICLE_H__

#include "vehicle.h"
#include "targetRingWidget.h"

namespace pammo
{

class FlameTankVehicle : 
    public Vehicle
{
public:
    FlameTankVehicle();
    virtual ~FlameTankVehicle();

    virtual int init();
    virtual void draw();
    virtual void update();
    virtual bool touch(uint32_t count, Touch* touches);
    
    void setFireDirection(float rads);

    void toggleTargetRing();

protected:
//    Image** mImages;
//    int mCurrImageIndex;

private:
    TargetRingWidget* mTargetRing;
    float mFireAngle;
    bool mTargetRingEnabled;
};

} // namespace pammo

#endif // 