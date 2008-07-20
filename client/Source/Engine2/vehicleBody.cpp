#include "vehicleBody.h"

namespace pammo
{

VehicleBody::VehicleBody() : Body()
{
    mTargetRotation = 0;
    mTargetAcceleration = 0;
    mCurrentAcceleration = 0;
    
    mProperties = 1;
    
    mRotationDamping = 0.4;
    mDamping = 0.05;
    
    mRadius = 20;
    mMass = 100;
}

VehicleBody::~VehicleBody()
{}

void VehicleBody::update()
{
    // Apply rotation accelerations.
    if(mTargetRotation != mRotation)
    {
        float diff = mTargetRotation - mRotation;
        float amt = fabs(diff);
        if(amt > M_PI) amt -= M_PI;
        amt /= 4;
        if(amt > 0.3) amt = 0.3;
        
        if(diff > 0 && diff < M_PI || diff < 0 && diff < -M_PI)
        {
            mRotationAcceleration += amt;
        }
        else
        {
            mRotationAcceleration -= amt;
        }
    }
    mRotation = mTargetRotation;
    
    // Apply linear accelerations.
    mCurrentAcceleration = mTargetAcceleration;
    mAcceleration += Vector2(mCurrentAcceleration, 0) * Transform2::createRotation(mRotation);
}

}