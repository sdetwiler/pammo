#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "pammo.h"
#include "view.h"
#include "targetRingWidget.h"
#include "weaponSelector.h"
#include "particleSystem.h" // For particle types.

#define PLAYER_MAX_IMAGE_COUNT 3

namespace pammo
{

struct Body;
class Camera;
class ImageEntity;
class VehicleController;
class Weapon;
class WeaponSelector;
class ScoreMeter;
class HealthMeter;
class DeathCardView;

class Player : 
    public View,
    public TargetRingObserver,
    public WeaponSelectorObserver
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
        virtual void onTargetRingUntouched(TargetRingWidget* widget);
                
        virtual void onWeaponSelectorUpdated(WeaponSelector* widget, Weapon* weapon);
        
		void damage(ParticleType type, float amount);

        void reset();
        void enable();
        void disable();

        void setCenter(Vector2 center);
        Vector2 const& getCenter() const;

        void setSpawnPoint(Vector2 const& p);
        Vector2 const& getSpawnPoint() const;
    public:
        TargetRingWidget* mMovementRing;
        TargetRingWidget* mTargetRing;
        
        WeaponSelector* mWeaponSelector;
        
        Body* mBody;
        VehicleController* mController;
        ImageEntity  mEntity;
        Image*       mImages[PLAYER_MAX_IMAGE_COUNT];
        uint32_t     mImageCount;
        uint32_t     mCurrImage;

        ImageEntity  mTurret;
        Vector2 mTurretTip;

        Weapon* mWeapon;
        
        HealthMeter* mHealthMeter;
        float mHealth;

        ScoreMeter* mScoreMeter;
        uint32_t mScore;

        uint64_t mDeadTime;

        bool mFiring;
        float mFireDirection;
        
        Vector2 mSpawnPoint;


        DeathCardView* mDeathCard;

	private:
		void createDust();
};

} // namespace pammo

#endif // __PLAYER_H__