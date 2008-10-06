#include "player.h"

#include "mainScreenView.h"
#include "deathCardView.h"

#include "world.h"
#include "camera.h"
#include "imageLibrary.h"
#include "imageEntity.h"
#include "targetRingWidget.h"
#include "particleSystem.h"
#include "vehicleController.h"
#include "physics.h"

//#include "flamethrowerWeapon.h"
#include "lightningWeapon.h"
#include "gooWeapon.h"
#include "grenadeLauncherWeapon.h"
#include "weaponSelector.h"

#include "enemyManager.h"
#include "healthMeter.h"
#include "scoreMeter.h"
#include "flipbookLoader.h"

#define MAX_CAMERA_SHAKE 15

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
    mDeathCard = NULL;
    mBody = NULL;
    mScoreMeter = new ScoreMeter();
    
    mHealthMeter = new HealthMeter(kHealthMeterPriority);
    mHealthMeter->setBaseLocation(Vector2(280, 16));
    
    mEnergyMeter = new HealthMeter(kEnergyMeterPriority);
    mEnergyMeter->setGrowDirection(-1);
    mEnergyMeter->setBaseLocation(Vector2(200, 16));

    mMovementRing = new TargetRingWidget(kMoveRingPriority, gImageLibrary->reference(INTERFACE_RING_MOVEMENT));
    mMovementRing->setCenter(Vector2(70, 250));
    mMovementRing->setSize(mMovementRing->getSize()*0.8);
    mMovementRing->setObserver(this);
    
    mTargetRing = new TargetRingWidget(kFireRingPriority, gImageLibrary->reference(INTERFACE_RING_TARGET));
    mTargetRing->setCenter(Vector2(410, 250));
    mTargetRing->setSize(mTargetRing->getSize()*0.8);
    mTargetRing->setObserver(this);

    //loadFlipbook("data/vehicles/tank/", mImages, PLAYER_MAX_IMAGE_COUNT, &mImageCount);
    mImageCount = VEHICLE_TANK_COUNT;
    loadFlipbook(VEHICLE_TANK_00, VEHICLE_TANK_COUNT, mImages);
    mCurrImage = 0;
    mEntity.setImage(mImages[mCurrImage]);

    //loadFlipbook("data/particles/shield/", mShieldImages, SHIELD_MAX_IMAGE_COUNT, &mShieldImageCount);
    mShieldImageCount = PARTICLE_SHIELD_COUNT;
    loadFlipbook(PARTICLE_SHIELD_00, PARTICLE_SHIELD_COUNT, mShieldImages);
    mShieldCurrImage = 0;
    mShieldFlipCount = 0;
    mShieldEntity.setImage(mShieldImages[mShieldCurrImage]);

    mTurret.setImage(gImageLibrary->reference(VEHICLE_TANK_TURRET_00));

    mController = new VehicleController();

    mWeaponSelector = new WeaponSelector();
    mWeaponSelector->setObserver(this);
    mGooWeapon = new GooWeapon();
    mLightningWeapon = new LightningWeapon();
    mGrenadeLauncherWeapon = new GrenadeLauncherWeapon();

    mShieldToggle = new ShieldToggle();
    mShieldToggle->setObserver(this);
    
    reset();
}

Player::~Player()
{
    mScoreMeter->destroy();
    mHealthMeter->destroy();
    mEnergyMeter->destroy();
    mWeaponSelector->destroy();
    mShieldToggle->destroy();
    mTargetRing->destroy();
    mMovementRing->destroy();
    delete mController;
}

void Player::reset()
{
    mCameraShake = 0;

    if(mBody)
    {
        gWorld->getPhysics()->removeBody(mBody);
    }
    mBody = gWorld->getPhysics()->addBody();
    
    mBody->mProperties = kPlayerCollisionProperties;
    mBody->mCollideProperties = kPlayerBarrierCollisionProperties;
    mBody->mUserArg = this;
    mBody->mDamping = 0.1;
    mBody->mRadius = 20;
    mBody->mMass = 100;

    mController->reset();
    mController->mBody = mBody;
    mController->mRotationDamping = 0.4f;
    
    mMovementRing->reset();
    mTargetRing->reset();
    mFiring = false;
    
    // Reset weapons.
    mWeaponSelector->reset();
    mWeaponSelector->addWeapon(new GooWeapon);
    mWeaponSelector->addWeapon(new LightningWeapon);
    mWeaponSelector->addWeapon(new GrenadeLauncherWeapon);

    // Reset life.
    mHealth = 1000.0f;
    mMaxHealth = 1000.0f;
    mDeadTime = 0;
    mHealthMeter->setPercent(mHealth);
    
    // Reset shield.
    mEnergy = 1000.0f;
    mMaxEnergy = 1000.0f;
    mEnergyMeter->setPercent(mEnergy);
    mShieldToggle->reset();

    // Reset misc.
    mScore = 0;
    mScoreMeter->setScore(mScore);


    setCenter(getSpawnPoint());
}

void Player::enable()
{
    mScoreMeter->enableAll();
    mHealthMeter->enableAll();
    mEnergyMeter->enableAll();
    mWeaponSelector->enableAll();
    mShieldToggle->enableAll();
    mTargetRing->enableAll();
    mMovementRing->enableAll();

    enableAll();
}

void Player::disable()
{
    mScoreMeter->disableAll();
    mHealthMeter->disableAll();
    mEnergyMeter->disableAll();
    mWeaponSelector->disableAll();
    mShieldToggle->disableAll();
    mTargetRing->disableAll();
    mMovementRing->disableAll();

    disableAll();
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
    p->mImage.setImage(gImageLibrary->reference(PARTICLE_DUST_00));
    p->mImage.mCenter = player->mBody->mCenter;
    p->mImage.mRotation = initialRotation + r;
	p->mImage.makeDirty();
}

void Player::update()
{
    mController->update();

    mEntity.mRotation = mController->mRotation + M_PI/2;
    mEntity.mCenter = mBody->mCenter;
    mEntity.makeDirty();
    
    Vector2 turretCenter = mBody->mCenter
                     + Vector2(-8.0f, 0.0f)
                     * Transform2::createRotation(mController->mRotation);


    mTurretTip = turretCenter
                     + Vector2(8.0f, 0.0f)
                     * Transform2::createRotation(mFireDirection);


    mTurret.mCenter = turretCenter;
    mTurret.mRotation = mFireDirection + M_PI/2;
    mTurret.makeDirty();
    
    int32_t shake = 0;
    if(mCameraShake)
    {
        shake = mCameraShake * rand()%2?-1:1;
        --mCameraShake;
    }

    gWorld->getCamera()->mCenter = mBody->mCenter + shake;
    gWorld->getCamera()->makeDirty();
 
    // Fire if we should be.
    if(mFiring)
    {
		mWeapon->fire();
    }

    float vmag = magnitude(mBody->mVelocity); 
    if(vmag > 7.0f)
    {
        mCurrImage = (mCurrImage+1) % mImageCount;
        mEntity.setImage(mImages[mCurrImage]);
    }
	if(vmag > 45.0f)
		createDust();
        
    if(mHealth < 1000)
    {
        //mHealth += 2;
        mHealthMeter->setPercent(mHealth);
    }
    
    // Shield.
    if(mShielding)
    {
        // Rate limit how often we flip.
        if(++mShieldFlipCount > 2)
        {
            mShieldCurrImage = (mShieldCurrImage+1) % mShieldImageCount;
            mShieldEntity.setImage(mShieldImages[mShieldCurrImage]);
            mShieldFlipCount = 0;
        }
        
        // Center the shield on the player.
        mShieldEntity.mCenter = mEntity.mCenter;
        mShieldEntity.makeDirty();
        
        // Subtract energy.
        mEnergy -= 1;
        if(mEnergy <= 0)
        {
            mEnergy = 0;
            mShieldToggle->setToggle(false);
        }
        mEnergyMeter->setPercent(mEnergy);
    }

    mScoreMeter->setScore(mScore);

    if(mHealth <= 0)
    {
        uint64_t now = getTime();
        if(!mDeadTime)
        {
            mDeadTime = now;
            mBody->mVelocity = Vector2(0,0);
            mBody->mCollideProperties = 0;
            mBody->mProperties = 0;
            mFiring = false;
            mController->reset();


        }

        if((now - mDeadTime) > 2000000)
        {
            if(!mDeathCard)
                mDeathCard = new DeathCardView;
        }

        if((now - mDeadTime) > 5000000)
        {
            gWorld->disable();
            gWorld->reset();
            mDeathCard->destroy();
            new MainScreenView;
        }

    }
}

void Player::draw()
{
    if(mDeadTime)
        return;

    gWorld->getCamera()->set();
    
    mEntity.draw();
    mTurret.draw();
    
    if(mShielding)
        mShieldEntity.draw();
    
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

void Player::onTargetRingTouched(TargetRingWidget *widget, float value)
{
    if(mDeadTime)
        return;

    if(widget == mMovementRing)
    {
        if(value < 0) value += M_PI*2;
        
        mController->mAcceleration = mBody->mMass * 7;
        mController->mRotationTarget = value;
    }
    else if(widget == mTargetRing)
    {
        mFiring = true;
        mFireDirection = value;
    }
}

void Player::onTargetRingUntouched(TargetRingWidget *widget)
{
    if(mDeadTime)
        return;

    if(widget == mMovementRing)
    {
        mController->mAcceleration = 0;
    }
    else if(widget == mTargetRing)
    {
        mFiring = false;
    }
}

void Player::onWeaponSelectorUpdated(WeaponSelector* widget, Weapon* weapon)
{
    mWeapon = weapon;
    mFiring = false;
}

void Player::damage(ParticleType type, float amount)
{
    if(mShielding)
    {
        dprintf("Shielding, no damage");
        return;
    }
    
    dprintf("damage");
	mHealth -= amount;
    mCameraShake = MAX_CAMERA_SHAKE;
    mHealthMeter->setPercent(mHealth);
}

void Player::onShieldToggleUpdated(ShieldToggle* widget, bool toggle)
{
    dprintf("Shield toggle callback");
    mShielding = toggle;
}

void Player::setSpawnPoint(Vector2 const& p)
{
    mSpawnPoint = p;
}

Vector2 const& Player::getSpawnPoint() const
{
    return mSpawnPoint;
}



}