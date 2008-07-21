#ifndef __VEHICLE_CONTROLLER_H__
#define __VEHICLE_CONTROLLER_H__

#include "pammo.h"

namespace pammo
{

class Body;

class VehicleController
{
    public:
        VehicleController();
        ~VehicleController();
        
        void update();
        
        float mRotationTarget;
        float mRotationDamping;
        float mRotationVelocity;
        float mRotation;
        
        float mAcceleration;
        
        Body* mBody;
};

}

#endif