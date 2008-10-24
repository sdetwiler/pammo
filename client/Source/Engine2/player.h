#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "pammo.h"
#include "view.h"
#include "imageLibrary.h"
#include "targetRingWidget.h"
#include "weaponSelector.h"
#include "shieldToggle.h"
#include "particleSystem.h" // For particle types.
#include "infastructureManager.h" // For powerup type.

#define PLAYER_MAX_IMAGE_COUNT    VEHICLE_TANK_COUNT
#define SHIELD_MAX_IMAGE_COUNT 3

namespace pammo
{

struct Body;
class Camera;
class ImageEntity;
class VehicleController;
class Weapon;
class ScoreMeter;
class HealthMeter;
class DeathCardView;
class GooWeapon;
class LightningWeapon;
class GrenadeLauncherWeapon;

class Player : 
    public View,
    public TargetRingObserver,
    public WeaponSelectorObserver,
    public ShieldToggleObserver
{
    public:
        Player();
        virtual ~Player();

        virtual uint32_t getTouchPriority() const;
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;

        virtual bool touch(uint32_t count, Touch* touches);
        virtual void update();
        virtual void draw();
        
        virtual void onTargetRingTouched(TargetRingWidget *widget, float value);
        virtual void onTargetRingDoubleTouched(TargetRingWidget* widget);
        virtual void onTargetRingUntouched(TargetRingWidget* widget);
                
        virtual void onWeaponSelectorUpdated(WeaponSelector* widget, Weapon* weapon);
        virtual void onShieldToggleUpdated(ShieldToggle* widget, bool toggle);
        
		void damage(ParticleType type, float amount);
        void givePowerup(PowerupType type);

        void reset();
        void enable();
        void disable();

        void setCenter(Vector2 center);
        Vector2 const& getCenter() const;

        void setSpawnPoint(Vector2 const& p);
        Vector2 const& getSpawnPoint() const;
    public:
        
        Body* mBody;
        VehicleController* mController;
        ImageEntity  mEntity;
        Image*       mImages[PLAYER_MAX_IMAGE_COUNT];
        ImageEntity  mShadow;
        uint32_t     mImageCount;
        uint32_t     mCurrImage;

        ImageEntity  mTurret;
        Vector2 mTurretTip;
        
        // Move / fire.
        TargetRingWidget* mMovementRing;
        TargetRingWidget* mTargetRing;
        float mFireDirection;
        bool mFiring;

        // Weapons.
        Weapon* mWeapon;
        WeaponSelector* mWeaponSelector;
        GooWeapon* mGooWeapon;
        LightningWeapon* mLightningWeapon;
        GrenadeLauncherWeapon* mGrenadeLauncherWeapon;
        
        // Health.
        HealthMeter* mHealthMeter;
        float mHealth, mMaxHealth;
        uint64_t mDeadTime;

        int32_t mCameraShake;
	
        // Energy.
		HealthMeter* mEnergyMeter;
        float mEnergy, mMaxEnergy;
        
        // Shield.
        ShieldToggle* mShieldToggle;
        bool mShielding, mHasShield;
        ImageEntity mShieldEntity;
        Image*      mShieldImages[SHIELD_MAX_IMAGE_COUNT];
        uint32_t    mShieldImageCount;
        uint32_t    mShieldCurrImage;
        uint32_t    mShieldFlipCount;
        
        // Score.
        ScoreMeter* mScoreMeter;
        uint32_t mScore;
        
        // Misc.
        Vector2 mSpawnPoint;
        DeathCardView* mDeathCard;

	private:
		void createDust(float scale);
};

} // namespace pammo

#endif // __PLAYER_H__
