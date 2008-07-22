#include "trebuchetEnemy.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "camera.h"
#include "imageLibrary.h"
#include "particleSystem.h"

namespace pammo
{
TrebuchetEnemy::TrebuchetEnemy(Vector2 const& position) 
    : Enemy(position)
{
    mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/trebuchet/00.png"));
}

TrebuchetEnemy::~TrebuchetEnemy()
{
    delete mEntity;
}

void TrebuchetEnemy::update()
{
    mController->update();

    mEntity->mRotation = mController->mRotation + M_PI/2;
    mEntity->mCenter = mBody->mCenter;
    mEntity->makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - mBody->mCenter;
    float mag = magnitude(heading);
    float speed = .5;
    float rot = atan2(heading.y, heading.x);
    if(rot < 0) 
        rot += M_PI*2;
    mController->mRotationTarget = rot;

    if(mag < 200)
    {
        mController->mAcceleration = 0;
        // Maybe fire.
        if(!(rand()%30))
            fire(mag);
    }
    else
    {
        mController->mAcceleration = speed * mBody->mMass * 7;
    }

    mEntity->makeDirty();

    /**
    Vector2 heading = gWorld->getPlayer()->getCenter() -  mBody->mCenter;
    
    float mag = magnitude(heading);

    float speed = .5;

    float rot = atan2(heading.y, heading.x);
    if(rot < 0) 
        rot += M_PI*2;

    mBody->mTargetRotation = rot;

    // If within range.
    if(mag < 200.0)
    {
        // Stop.
        mBody->mTargetAcceleration = 0;
        
        // Maybe fire.
        if(!(rand()%30))
            fire(mag);
    }
    else
    {
        mBody->mTargetAcceleration = speed * mBody->mMass * 7;
    }

    mBody->update();

    mEntity->mRotation = mBody->mRotation + M_PI/2;
    mEntity->mCenter = mBody->mCenter;

    mEntity->makeDirty();
    **/
}


void TrebuchetEnemy::fire(float distance)
{
    Vector2 baseVelocity;
    float rad = 32.0f; // Distance from center of vehicle to end of trebuchet.
    float rot = mEntity->mRotation - M_PI/2;
    
    // Calculate center. Vehicle center plus arm rotated for direction.
    Vector2 center = mEntity->mCenter + Vector2(rad, 0) * Transform2::createRotation(rot);

    ParticleSystem::InitBallParticleArgs args;
    args.initialPosition = center;
    args.initialRotation = rot;
    args.initialVelocity = mBody->mVelocity;
    args.maxDistance = distance;

    gWorld->getParticleSystem()->initBallParticle(args);
}


} // namespace pammo