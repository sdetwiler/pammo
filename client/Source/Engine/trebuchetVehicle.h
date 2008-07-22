#ifndef __TREBUCHETVEHICLE_H__
#define __TREBUCHETVEHICLE_H__

#include "vehicle.h"
#include "magnitudeWidget.h"

namespace pammo
{

#define VEHICLE_TREBUCHET 2
class TrebuchetVehicle : 
    public Vehicle
{
public:
    TrebuchetVehicle();
    virtual ~TrebuchetVehicle();

    virtual int init();
    virtual void draw();
    virtual void update();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void hit(float damage);
    virtual void destroy();
    virtual uint32_t getTypeId();
    void fire();

    static void particleHitCb(Vehicle* vehicle, void* arg);

    void setFireDirection(float rads);
    float getFireDirection() const;

    void setFireMagnitude(float mag);
    float getFireMagnitude() const;

    void toggleTargetRing();

protected:
    void onParticleHit(Vehicle* vehicle);

private:
    MagnitudeWidget* mMagnitudeWidget;
    float mFireAngle;
    float mMagnitude;
};

} // namespace pammo

#endif // 