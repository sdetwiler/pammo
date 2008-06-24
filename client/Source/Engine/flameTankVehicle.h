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
    virtual void hit(float damage);
    virtual void destroy();

    static void particleHitCb(Vehicle* vehicle, void* arg);

    void setFireDirection(float rads);
    float getFireDirection();

    void toggleTargetRing();

protected:
    void onParticleHit(Vehicle* vehicle);

private:
    TargetRingWidget* mTargetRing;
    float mFireAngle;
    bool mTargetRingEnabled;
};

} // namespace pammo

#endif // 