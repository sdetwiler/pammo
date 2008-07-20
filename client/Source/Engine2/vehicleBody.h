#ifndef __VEHICLE_BODY_H__
#define __VEHICLE_BODY_H__

#include "pammo.h"
#include "body.h"

namespace pammo
{

class VehicleBody : public Body
{
    public:
        VehicleBody();
        virtual ~VehicleBody();
        
        void update();
        
        float mTargetRotation;
        float mTargetAcceleration;
        
    private:
        float mCurrentAcceleration;
};

}

#endif