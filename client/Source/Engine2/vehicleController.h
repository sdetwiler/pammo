#ifndef __VEHICLE_CONTROLLER_H__
#define __VEHICLE_CONTROLLER_H__

#include "pammo.h"

namespace pammo
{

struct Body;

class VehicleController
{
    public:
        VehicleController();
        virtual ~VehicleController();
        
        void reset();
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