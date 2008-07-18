#ifndef __VEHICLE_BODY_H__
#define __VEHICLE_BODY_H__

#include "pammo.h"

namespace pammo
{

class VehicleBody
{
    public:
        VehicleBody();
        ~VehicleBody();
        
        void update();
        
        float mTargetRotation;
        float mVelocityRotation;
        float mRotation;
        
        float mTargetAcceleration;
        float mAcceleration;
        Vector2 mVelocity;
        Vector2 mCenter;
};

}

#endif