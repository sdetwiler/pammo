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

    mTurretImage = gImageLibrary->reference("data/vehicles/flameTank5/turret/00.png");
    if(!mTurretImage)
        return -1;

    mCenterImage = gImageLibrary->reference("data/vehicles/center.png");

    // SCD TEMP
    //mCenter = Vector2(350, 350);
    return 0;
}


void FlameTankVehicle::draw()
{
    Vehicle::draw();

    Transform2 trans;
    Vector2 center = mCenter + (Vector2(0, -8) * Transform2::createRotation(mRotation)); // Center of turret.
    trans*= Transform2::createTranslation(center);         // Go to center of vehicle.
    trans*= Transform2::createRotation(mFireAngle);        // Rotate in firing direction.
    trans*= Transform2::createTranslation(Vector2(-8, -8)); // Translate to center of turret image.
    trans*= Transform2::createScale(16); // scale image to correct size.
    
    drawImage(mTurretImage, trans, 1.0f);
    

    if(mTargetRingEnabled)
        mTargetRing->draw();
    
    // SCD Debug Max fire distance begin.
    /***
    float maxDistance = 150.0f;
    Vector2 end = mCenter + Vector2(maxDistance, 0) * Transform2::createRotation(mFireAngle- 90.0f*0.0174532925f);
    glPushMatrix();
    glLoadIdentity();
    glLineWidth(5.0);
    glColor4f(0.0, 1.0, 0.0, 0.5);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	float tmp[2*2];// = new float[2*2];
    tmp[0] = this->mCenter.x+10;
    tmp[1] = this->mCenter.y-16;
    tmp[2] = end.x+10;
    tmp[3] = end.y-16;
    glVertexPointer(2, GL_FLOAT, 0, tmp);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
    glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // SCD Debug end.
    ***/
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

    float rot = mFireAngle + ((rand()%3)/90.0f);

    Transform2 trans;
    Vector2 center = mCenter + (Vector2(0, -8) * Transform2::createRotation(mRotation));// + Vector2((rand()%6)-3, (rand()%6)-3); // Center of turret.
    trans*= Transform2::createTranslation(center);         // Go to center of vehicle.
    trans*= Transform2::createRotation(rot);        // Rotate in firing direction.
    trans*= Transform2::createTranslation(Vector2(0, -24)); // Translate to center of turret image.
    trans*= Transform2::createScale(16); // scale image to correct size.

    Vector2 start;
    start = start * trans;

    if(mMoving)
        baseVelocity = Vector2(mSpeed, 0) * Transform2::createRotation(mFireAngle - 90.0f*0.0174532925f);
    else
        baseVelocity = Vector2(0, 0);

    ParticleSystem::InitFireParticleArgs args;
    args.emitter = this;
    args.hitCallback = particleHitCb;
    args.hitCallbackArg = this;
    args.initialPosition = start;
    args.initialRotation = mFireAngle - 90.0f*0.0174532925f;
    args.initialVelocity = baseVelocity;

    gWorld->getParticleSystem()->initSmokeParticle(start, mFireAngle - 90.0f*0.0174532925f, baseVelocity);
    gWorld->getParticleSystem()->initFireParticle(args);
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

uint32_t FlameTankVehicle::getTypeId()
{
    return VEHICLE_FLAMETANK;
}

} // namespace pammo
