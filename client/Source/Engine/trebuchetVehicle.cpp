#include "pammo.h"
#include "imageLibrary.h"
#include "world.h"
#include "trebuchetVehicle.h"
#include "collisionDynamics.h"
#include "camera.h"

#include <stdio.h>
#include <stdlib.h>

#include <set>
#include <string>

namespace pammo
{

TrebuchetVehicle::TrebuchetVehicle()
    : Vehicle()
{
    mFireAngle = 0;
    mMagnitude = 0;
}

TrebuchetVehicle::~TrebuchetVehicle()
{
    gWorld->getCollisionDynamics()->removeVehicle(this);
}

int TrebuchetVehicle::init()
{
    int ret;
    ret = Vehicle::init();
    if(ret < 0)
        return ret;

    mMagnitudeWidget = new MagnitudeWidget(this);
    ret = mMagnitudeWidget->init();
    if(ret < 0)
        return -1;

    ret = mAnimation.init("data/vehicles/trebuchet/");
    if(ret < 0)
        return -1;

    mAnimation.setDrawPriority(100);
    setSize(mAnimation.getSize());

    // SCD TEMP
    //mCenter = Vector2(350, 350);
    return 0;
}


void TrebuchetVehicle::draw()
{
    Vehicle::draw();

    mMagnitudeWidget->draw();
}

void TrebuchetVehicle::setFireDirection(float rads)
{
    mFireAngle = rads;
}

float TrebuchetVehicle::getFireDirection() const
{
    return mFireAngle;
}

void TrebuchetVehicle::setFireMagnitude(float mag)
{
    mMagnitude = mag;
}

float TrebuchetVehicle::getFireMagnitude() const
{
    return mMagnitude;
}

void TrebuchetVehicle::update()
{
    Vehicle::update();

    getTransform();
}

bool TrebuchetVehicle::touch(uint32_t count, Touch* touches)
{
    // Always only use first touch point.
    Vector2 loc = gWorld->getCamera()->translateToWorld(touches[0].mLocation);
    float x = mCenter.x - loc.x;
    float y = mCenter.y - loc.y;
    float hyp = sqrt(x*x + y*y);

    return mMagnitudeWidget->touch(count, touches);

    return false;
}

void TrebuchetVehicle::destroy()
{
}

void TrebuchetVehicle::fire()
{
    Vector2 baseVelocity;
    float rad = 32.0f; // Distance from center of vehicle to end of trebuchet.
    float rot = mFireAngle + 90.0f*0.0174532925f;
    
    // Calculate center. Vehicle center plus arm rotated for direction.
    Vector2 center = mCenter + Vector2(rad, 0) * Transform2::createRotation(rot);

    // Calculate base / initial velocity. Vehicle speed rotated for direction.
    if(mMoving)
        baseVelocity = Vector2(mSpeed, 0) * Transform2::createRotation(rot);
    else
        baseVelocity = Vector2(0, 0);

    ParticleSystem::InitBallParticleArgs args;
    args.emitter = this;
    args.hitCallback = particleHitCb;
    args.hitCallbackArg = this;
    args.initialPosition = center;
    args.initialRotation = rot;
    args.initialVelocity = baseVelocity;
    args.maxDistance = this->getFireMagnitude();

    gWorld->getParticleSystem()->initBallParticle(args);

}

void TrebuchetVehicle::hit(float damage)
{
    if(getObserver())
        getObserver()->onHit(this, damage);
}

void TrebuchetVehicle::onParticleHit(Vehicle* vehicle)
{
    if(vehicle)
        vehicle->hit(100.0f);
}

void TrebuchetVehicle::particleHitCb(Vehicle* vehicle, void* arg)
{
    ((TrebuchetVehicle*)arg)->onParticleHit(vehicle);
}

uint32_t TrebuchetVehicle::getTypeId()
{
    return VEHICLE_TREBUCHET;
}

} // namespace pammo
