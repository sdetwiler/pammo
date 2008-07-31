#include "vehicleController.h"
#include "physics.h"

namespace pammo
{

VehicleController::VehicleController()
{
    reset();
}

VehicleController::~VehicleController()
{}

void VehicleController::reset()
{
    mBody = 0;
    
    mRotationTarget = 0;
    mRotationDamping = 0;
    mRotationVelocity = 0;
    mRotation = 0;
    
    mAcceleration = 0;
}

void VehicleController::update()
{
    if(!mBody) return;
    
    // Apply rotation accelerations.
    if(mRotationTarget != mRotation)
    {
        float diff = mRotationTarget - mRotation;
        float amt = fabs(diff);
        if(amt > M_PI) amt -= M_PI;
        amt /= 4;
        if(amt > 0.3) amt = 0.3;
        
        if(diff > 0 && diff < M_PI || diff < 0 && diff < -M_PI)
        {
            mRotationVelocity += amt;
        }
        else
        {
            mRotationVelocity -= amt;
        }
    }
    
    // Integrate rotation.
    mRotationVelocity *= 1 - mRotationDamping;
    mRotation += mRotationVelocity;
    
    if(mRotation > 2*M_PI) mRotation -= 2*M_PI;
    if(mRotation < 0) mRotation += 2*M_PI;
    
    // Apply linear accelerations.
    mBody->mAcceleration += Vector2(mAcceleration, 0) * Transform2::createRotation(mRotation);
}

}