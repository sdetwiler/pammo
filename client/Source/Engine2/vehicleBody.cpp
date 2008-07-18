#include "vehicleBody.h"

namespace pammo
{

VehicleBody::VehicleBody()
{
    mTargetRotation = 0;
    mVelocityRotation = 0;
    mRotation = 0;
    
    mTargetAcceleration = 0;
    mAcceleration = 0;
    mVelocity = Vector2(0, 0);
    mCenter = Vector2(0, 0);
}

VehicleBody::~VehicleBody()
{}

void VehicleBody::update()
{
    // Advance rotation.
    if(mTargetRotation != mRotation)
    {
        float diff = mTargetRotation - mRotation;
        float amt = fabs(diff);
        if(amt > M_PI) amt -= M_PI;
        amt /= 4;
        if(amt > 0.3) amt = 0.3;
        
        if(diff > 0 && diff < M_PI || diff < 0 && diff < -M_PI)
        {
            mVelocityRotation += amt;
        }
        else
        {
            mVelocityRotation -= amt;
        }
    }
    
    mVelocityRotation *= 0.6;
    mRotation += mVelocityRotation;
    
    if(mRotation > 2*M_PI) mRotation -= 2*M_PI;
    if(mRotation < 0) mRotation += 2*M_PI;

    mVelocity += Vector2(mTargetAcceleration, 0) * Transform2::createRotation(mRotation);
    mVelocity *= 0.9;
    mCenter += mVelocity;
}

}