#include "pammo.h"
#include "imageLibrary.h"
#include "world.h"
#include "flameTankVehicle.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <string>

namespace pammo
{

FlameTankVehicle::FlameTankVehicle()
    : Vehicle()
{
    mFireAngle = 0;
    mTargetRingEnabled = 0;
}

FlameTankVehicle::~FlameTankVehicle()
{

}

int FlameTankVehicle::init()
{
    int ret;
    ret = Vehicle::init();
    if(ret < 0)
        return ret;

    mTargetRing = new TargetRingWidget(this);
    ret = mTargetRing->init();
    if(ret < 0)
        return -1;

    ret = mAnimation.init("data/vehicles/flameTank4/");
    if(ret < 0)
        return -1;

    mAnimation.setDrawPriority(100);
    setSize(mAnimation.getSize());
    // SCD TEMP
    //mCenter = Vector2(350, 350);
    return 0;
}


void FlameTankVehicle::draw()
{
    Vehicle::draw();

    if(mTargetRingEnabled)
        mTargetRing->draw();
}

void FlameTankVehicle::setFireDirection(float rads)
{
    mFireAngle = rads;
}

float FlameTankVehicle::getFireDirection()
{
    return mFireAngle;
}

void FlameTankVehicle::update()
{
    Vehicle::update();

    getTransform();

    Vector2 baseVelocity;

    // Fire.
    int width=1;
    int i=0;
    //for(int i= -width; i<width; ++i)
    {
        // Add new flame particle.
        float rad = 20.0f; // Distance from center of tank to end of nozzle.
        float rot = mFireAngle + mRotation-(90.0f*0.0174532925f)+(i*.04f) ;//+ ((rand()%3)/20.0f);
       
        Transform2 trans;
        Vector2 center = mCenter;
//        center.x +=((rand()%6)-3);
//        center.y +=((rand()%6)-3);
        

        trans *= Transform2::createTranslation(center);
        trans *= Transform2::createRotation(rot);
        
        // Position of the tip of the nozzle.
        trans*= Transform2::createTranslation(Vector2(rad,0));

        baseVelocity.x=0;
        baseVelocity.y=0;
        if(mMoving)
        {
            baseVelocity.x = mSpeed;
            baseVelocity *= Transform2::createRotation(rot);
        }

       // if(!(rand()%100))
            gWorld->getParticleSystem()->initFireParticle(trans, baseVelocity);
    }

    // Smoke.
    width=1;
    for(int i= -width; i<=width; ++i)
    {
        float rad = 20; // Distance from center of tank to end of nozzle.
        //float rot = mRotation-(90.0*0.0174532925)+(i*.04f) + ((rand()%3)/20.0);
        float rot = mFireAngle + mRotation-(90.0f*0.0174532925f)+(i*.06f) + (-0.3f +(rand()%10)/15.0f);
       
        Transform2 trans;
        trans = Transform2::createTranslation(mCenter);
        trans*= Transform2::createRotation(rot);
        
        // Position of the tip of the nozzle.
        trans*= Transform2::createTranslation(Vector2(rad,0));

        baseVelocity.x=0;
        baseVelocity.y=0;
        if(mMoving)
        {
            baseVelocity.x = mSpeed;
            baseVelocity *= Transform2::createRotation(rot);
        }

        gWorld->getParticleSystem()->initSmokeParticle(trans, baseVelocity);
    }

}

void FlameTankVehicle::toggleTargetRing()
{
    mTargetRingEnabled = !mTargetRingEnabled;
}

bool FlameTankVehicle::touch(uint32_t count, Touch* touches)
{
    // Always only use first touch point.
    Vector2 loc = gWorld->getCamera()->translateToWorld(touches[0].mLocation);
    float x = mCenter.x - loc.x;
    float y = mCenter.y - loc.y;
    float hyp = sqrt(x*x + y*y);

    // Inside of vehicle.
    if(hyp < 32.0f)
    {
        toggleTargetRing();
    }
    if(mTargetRingEnabled)
        return mTargetRing->touch(count, touches);

    return false;
}


} // namespace pammo