#include "pammo.h"
#include "imageLibrary.h"
#include "world.h"
#include "flameTankVehicle.h"
#include "collisionDynamics.h"
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
    mTargetRingEnabled = false;
}

FlameTankVehicle::~FlameTankVehicle()
{
    gWorld->getCollisionDynamics()->removeVehicle(this);
}

void FlameTankVehicle::setTargetRingState(bool enabled)
{
    mTargetRingEnabled = enabled;
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

    ret = mAnimation.init("data/vehicles/flameTank5/");
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
    int i=1;
    //for(int i= 0; i<width; ++i)
    {
        // Add new flame particle.
        float rad = 32.0f; // Distance from center of tank to end of nozzle.
        float rot = mFireAngle - 90.0f*0.0174532925f +(i*.02f) + ((rand()%3)/60.0f);
        
        // Calculate center. Vehicle center plus nozzle rad rotated for direction.
        Vector2 center = mCenter + Vector2(rad, 0) * Transform2::createRotation(rot) + Vector2((rand()%6)-3, (rand()%6)-3);

        // Calculate base / initial velocity. Vehicle speed rotated for direction.
        if(mMoving)
            baseVelocity = Vector2(mSpeed, 0) * Transform2::createRotation(rot);
        else
            baseVelocity = Vector2(0, 0);

        ParticleSystem::InitFireParticleArgs args;
        args.emitter = this;
        args.hitCallback = particleHitCb;
        args.hitCallbackArg = this;
        args.initialPosition = center;
        args.initialRotation = rot;
        args.initialVelocity = baseVelocity;

        gWorld->getParticleSystem()->initFireParticle(args);
    }

    // Smoke.
    width=1;
    //for(int i= 0; i<=width; ++i)
    {
        float rad = 20; // Distance from center of tank to end of nozzle.
        float rot = mFireAngle - 90.0f*0.0174532925f+ (i*.06f) + (-0.3f +(rand()%10)/15.0f);
        
        // Calculate center. Vehicle center plus nozzle rad rotated for direction.
        Vector2 center = mCenter + Vector2(rad, 0) * Transform2::createRotation(rot);

        // Calculate base / initial velocity. Vehicle speed rotated for direction.
        if(mMoving)
            baseVelocity = Vector2(mSpeed, 0) * Transform2::createRotation(rot);
        else
            baseVelocity = Vector2(0, 0);

        gWorld->getParticleSystem()->initSmokeParticle(center, rot, baseVelocity);
    }
}

bool FlameTankVehicle::touch(uint32_t count, Touch* touches)
{
    return mTargetRing->touch(count, touches);
}

void FlameTankVehicle::destroy()
{
}

void FlameTankVehicle::hit(float damage)
{
    if(getObserver())
        getObserver()->onHit(this, damage);
}

void FlameTankVehicle::onParticleHit(Vehicle* vehicle)
{
    if(vehicle)
        vehicle->hit(5.0f);
}

void FlameTankVehicle::particleHitCb(Vehicle* vehicle, void* arg)
{
    ((FlameTankVehicle*)arg)->onParticleHit(vehicle);
}

} // namespace pammo
