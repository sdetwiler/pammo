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
    
    if(mRotation >= 2*M_PI) mRotation -= 2*M_PI;
    else if(mRotation < 0) mRotation += 2*M_PI;
    
    if(mRotationTarget >= 2*M_PI) mRotationTarget -= 2*M_PI;
    else if(mRotationTarget < 0) mRotationTarget += 2*M_PI;
    
    // Apply rotation accelerations.
    if(mRotationTarget != mRotation)
    {
        float diff = mRotationTarget - mRotation;
        if(diff > M_PI) diff = diff - 2*M_PI;
        else if(diff < -M_PI) diff = diff + 2*M_PI;
        
        diff /= 4;
        if(diff > 0.3) diff = 0.3;
        else if(diff < -0.3) diff = -0.3;
        
        mRotationVelocity += diff;
    }
    
    // Integrate rotation.
    mRotationVelocity *= 1 - mRotationDamping;
    mRotation += mRotationVelocity;
    
    // Apply linear accelerations.
    mBody->mAcceleration += Vector2(mAcceleration, 0) * Transform2::createRotation(mRotation);
}

}