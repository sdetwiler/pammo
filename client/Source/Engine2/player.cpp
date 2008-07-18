#include "player.h"
#include "world.h"
#include "camera.h"
#include "imageLibrary.h"
#include "imageEntity.h"
#include "targetRingWidget.h"
#include "particleSystem.h"

namespace pammo
{

Player::Player() : View()
{
    mMovementRing = new TargetRingWidget(kMoveRingPriority);
    mMovementRing->setObserver(this);
    mMovementRing->setCenter(Vector2(160, 70));
    
    mTargetRing = new TargetRingWidget(kFireRingPriority);
    mTargetRing->setObserver(this);
    mTargetRing->setCenter(Vector2(160, 410));
    
    mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/flameTank5/00.png"));
    mEntity->mCenter = Vector2(3000, 1500);
    
    mTargetRotation = 0;
    mVelocityRotation = 0;
    mRotation = 0;
    
    mTargetAcceleration = 0;
    mAcceleration = 0;
    
    mVelocity = Vector2(0, 0);
    
    mFiring = false;
}

Player::~Player()
{
}

uint32_t Player::getTouchPriority() const
{
    return kPlayerPriority;
}

uint32_t Player::getUpdatePriority() const
{
    return kPlayerPriority;
}

uint32_t Player::getDrawPriority() const
{
    return kPlayerPriority;
}

bool Player::touch(uint32_t count, Touch* touches)
{
    return false;
}

void Player::update()
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
        
    mEntity->mRotation = mRotation + M_PI/2;
    mEntity->makeDirty();

    mVelocity += Vector2(mTargetAcceleration, 0) * Transform2::createRotation(mRotation);
    
    mVelocity *= 0.9;

    mEntity->mCenter += mVelocity;
    
    gWorld->getCamera()->mCenter = mEntity->mCenter;
    gWorld->getCamera()->makeDirty();
    
    // Fire if we should be.
    if(mFiring)
    {
        ParticleSystem::InitFireParticleArgs args;
        args.initialPosition = mEntity->mCenter;
        args.initialRotation = atan2(mFireDirection.y, mFireDirection.x);
        args.initialVelocity = mVelocity;
        
        gWorld->getParticleSystem()->initFireParticle(args);
    }
}

void Player::draw()
{
    gWorld->getCamera()->set();
    
    mEntity->draw();
    
    gWorld->getCamera()->unset();
}

void Player::setCenter(Vector2 center)
{
    mEntity->mCenter = center;
    mEntity->makeDirty();
}

void Player::onTargetRingUpdated(TargetRingWidget *widget, Vector2 value)
{
    if(widget == mMovementRing)
    {
        float mag = magnitude(value);
        float rot = atan2(value.y, value.x);
        if(mag < 0.1)
        {
            mTargetAcceleration = 0;
        }
        else
        {
            if(rot < 0) rot += M_PI*2;
            mTargetAcceleration = mag;
            mTargetRotation = rot;
        }
    }
    else if(widget == mTargetRing)
    {
        float mag = magnitude(value);
        if(mag < 0.1)
            mFiring = false;
        else
        {
            mFiring = true;
            mFireDirection = value;
        }
    }
}

}