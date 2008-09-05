#include "player.h"

#include "world.h"
#include "camera.h"
#include "imageLibrary.h"
#include "imageEntity.h"
#include "targetRingWidget.h"
#include "particleSystem.h"
#include "vehicleController.h"
#include "physics.h"
#include "flamethrowerWeapon.h"
#include "lightningWeapon.h"
#include "enemyManager.h"
#include "weaponSelector.h"
#include "healthMeter.h"
#include "flipbookLoader.h"

namespace pammo
{

void dustParticleCallback(Particle* p, ParticleSystem* system)
{
    //p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= 1.1;
    p->mImage.makeDirty();
    
    p->mAlpha-=0.05f;
    if(p->mAlpha <= 0)
        system->removeParticle(p);
}


Player::Player() : View()
{
    mMovementRing = new TargetRingWidget(kMoveRingPriority);
    mMovementRing->setObserver(this);
    mMovementRing->setCenter(Vector2(60, 260));
    
    mTargetRing = new TargetRingWidget(kFireRingPriority);
    mTargetRing->setObserver(this);
    mTargetRing->setCenter(Vector2(420, 260));

    //mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/tank/00.png"));
    loadFlipbook("data/vehicles/tank/", mImages, PLAYER_MAX_IMAGE_COUNT, &mImageCount);
    mCurrImage = 0;
    mEntity.setImage(mImages[mCurrImage]);
    mBody = gWorld->getPhysics()->addBody();
    
    mBody->mProperties = kPlayerCollisionProperties;
    mBody->mCollideProperties = kPlayerBarrierCollisionProperties;
    mBody->mUserArg = this;
    mBody->mDamping = 0.1;
    mBody->mRadius = 20;
    mBody->mMass = 100;
    
    mController = new VehicleController();
    mController->mBody = mBody;
    mController->mRotationDamping = 0.4;

    mWeaponSelector = new WeaponSelector();
    mWeaponSelector->setObserver(this);
    mWeaponSelector->addWeapon(new LightningWeapon);
    mWeaponSelector->addWeapon(new FlamethrowerWeapon);
    
    mScore = 0;  
    mHealth = 1000.0;
    mHealthMeter = new HealthMeter();
    mHealthMeter->setPercent(mHealth);
    
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

void Player::createDust()
{
	    // Get a particle.
	Particle* p = gWorld->getParticleSystem()->addParticle(1);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = dustParticleCallback;
    p->mAlpha = .65f;
    
    // Choose some numbers.
    float f = 4.0;
    float r = 1.0/f - ((rand()%100)/(f*50)) ;
    float initialRotation = atan2(mBody->mVelocity.y, mBody->mVelocity.x );
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/dust00.png"));
    p->mImage.mCenter = player->mBody->mCenter;
    p->mImage.mRotation = initialRotation + r;
	p->mImage.makeDirty();
}

void Player::update()
{
    mController->update();
    float vmag = magnitude(mBody->mVelocity); 

    if(vmag > 7.0f)
    {
        mCurrImage = (mCurrImage+1) % mImageCount;
        mEntity.setImage(mImages[mCurrImage]);
    }

    mEntity.mRotation = mController->mRotation + M_PI/2;
    mEntity.mCenter = mBody->mCenter;
    mEntity.makeDirty();
    
    gWorld->getCamera()->mCenter = mBody->mCenter;
    gWorld->getCamera()->makeDirty();
 
    // Fire if we should be.
    if(mFiring)
    {
		mWeapon->fire();
    }

	if(vmag > 45.0f)
		createDust();
        
    if(mHealth < 1000)
    {
        mHealth += 2;
        mHealthMeter->setPercent(mHealth);
    }
}

void Player::draw()
{
    gWorld->getCamera()->set();
    
    mEntity.draw();
    
    gWorld->getCamera()->unset();
}

void Player::setCenter(Vector2 center)
{
    mBody->mCenter = center;
}

Vector2 const& Player::getCenter() const
{
    return mEntity.mCenter;
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
            //mController->mAcceleration = mag * mBody->mMass * 7;
            mController->mAcceleration = mBody->mMass * 7;
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


void Player::onWeaponSelectorUpdated(WeaponSelector* widget, Weapon* weapon)
{
    mWeapon = weapon;
}

void Player::damage(ParticleType type, float amount)
{
	mHealth -= amount;
    mHealthMeter->setPercent(mHealth);
}

}