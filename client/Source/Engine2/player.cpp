#include "pammo.h"
#include "player.h"
#include "world.h"
#include "camera.h"
#include "imageLibrary.h"
#include "imageEntity.h"
#include "targetRingWidget.h"
#include "particleSystem.h"
#include "vehicleController.h"
#include "physics.h"
#include "enemyManager.h"

namespace pammo
{

Player::Player() : View()
{
    mMovementRing = new TargetRingWidget(kMoveRingPriority);
    mMovementRing->setObserver(this);
    
    mTargetRing = new TargetRingWidget(kFireRingPriority);
    mTargetRing->setObserver(this);

    Vector2 size = getFrameSize();
    if(size.x > size.y)
    {
        mMovementRing->setCenter(Vector2(70, 160));
        mTargetRing->setCenter(Vector2(410, 160));
    }
    else
    {
        mMovementRing->setCenter(Vector2(160, 70));
        mTargetRing->setCenter(Vector2(160, 410));
    }

    mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/flameTank5/00.png"));
    
    mBody = gWorld->getPhysics()->addBody();
	mBody->mCenter = *(gWorld->getEnemyManager()->getSpawnPoint(0));
    
    mBody->mProperties = kPlayerCollisionProperties;
    mBody->mDamping = 0.1;
    mBody->mRadius = 20;
    mBody->mMass = 100;
    
    mController = new VehicleController();
    mController->mBody = mBody;
    mController->mRotationDamping = 0.4;
    
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
    mController->update();

    mEntity->mRotation = mController->mRotation + M_PI/2;
    mEntity->mCenter = mBody->mCenter;
    mEntity->makeDirty();
    
    gWorld->getCamera()->mCenter = mBody->mCenter;
    gWorld->getCamera()->makeDirty();
 
    // Fire if we should be.
    if(mFiring)
    {
        ParticleSystem::InitFireParticleArgs args;
        args.initialPosition = mBody->mCenter;
        args.initialRotation = atan2(mFireDirection.y, mFireDirection.x);
        args.initialVelocity = mBody->mVelocity;
        
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

Vector2 const& Player::getCenter() const
{
    return mEntity->mCenter;
}

void Player::onTargetRingUpdated(TargetRingWidget *widget, Vector2 value)
{
    if(widget == mMovementRing)
    {
        float mag = magnitude(value);
        float rot = atan2(value.y, value.x);
        if(mag < 0.1)
        {
            mController->mAcceleration = 0;
        }
        else
        {
            if(rot < 0) rot += M_PI*2;
            mController->mAcceleration = mag * mBody->mMass * 7;
            mController->mRotationTarget = rot;
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

void Player::damage(ParticleType type, float amount)
{
	dprintf("ouch");
}

}