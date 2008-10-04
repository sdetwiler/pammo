#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "pammo.h"
#include "view.h"

#include "vehicleController.h"
#include "imageEntity.h"
#include "particleSystem.h"
#include <vector>

#define ENEMY_MAX_WEAPON_COUNT 4
#define ENEMY_MAX_PARTICLE_COUNT 4
#define ENEMY_MAX_IMAGE_COUNT 8

namespace pammo
{

class EnemyManager;
struct Enemy;
struct EnemyWeapon;
struct EnemyParticle;
struct TurretWeaponData;

typedef void (*EnemyBehaviorCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyWeaponCb)(Enemy* e, EnemyWeapon* w, EnemyManager* manager);
typedef void (*EnemyParticleCb)(Enemy* e, EnemyParticle* w, EnemyManager* manager);

typedef void (*EnemyUpdateCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDrawCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDamageCb)(Enemy* e, ParticleType type, float amount);
typedef void (*EnemyDestroyCb)(Enemy* e, EnemyManager* manager);

float clamp(float v, float min, float max);
void enemyWeaponTurretUpdate(Enemy* e, EnemyWeapon* w, EnemyManager* manager, TurretWeaponData* data);
void enemyWeaponTurretGetParticleWithBody(Enemy* e, EnemyWeapon* w, EnemyManager* manager, TurretWeaponData* data, Particle** p);

//////////////////////////////////////////////////////////
//// Weapons.
//////////////////////////////////////////////////////////

enum WeaponType
{
    None,
    Flamethrower,
    MachineGun,
    Trebuchet,
    SelfDestruct,
    HeatSeaker,
    MineLayer
};

struct EnemyWeapon
{
	WeaponType     mType;
    ImageEntity    mEntity;
    EnemyWeaponCb  mCb;

    uint8_t        mData[256];
};

struct EnemyWeaponTemplate
{
    EnemyWeapon  mWeapon;
    char         mImagePath[256];
};

struct TurretWeaponData
{
    Vector2        mPosition;
    Vector2        mFirePosition;
    float          mRotationMin;
    float          mRotationMax;

    Vector2        mTurretCenter;
    Vector2        mTurretTip;
    float          mTurretRotation;
};

struct FlamethrowerWeaponData
{
    TurretWeaponData mTurret;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mMaxDistance;
    uint32_t mSpreadAngle;
    float    mFireRate;
    uint64_t mFireStateChange;
    uint64_t mFiring;
};

struct TrebuchetWeaponData
{
    TurretWeaponData mTurret;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mMaxDistance;
    float mFireRate;

    uint64_t mLastFire;
};

struct MachineGunWeaponData
{
    TurretWeaponData mTurret;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mMaxDistance;
    float    mFireRate;
    uint64_t mFireStateChange;
    uint64_t mFiring;
};

struct HeatSeakerWeaponData
{
    TurretWeaponData mTurret;
    float    mAccuracy;
    uint32_t mDamage;
    uint32_t mMaxDistance;
    float    mFireRate;
    uint64_t mFireStateChange;
    uint64_t mFiring;
};

struct SelfDestructWeaponData
{
    uint32_t mDamage;
};

struct MineLayerWeaponData
{
    uint32_t mDamage;
    uint64_t mMaxLifetime; // in usec
    float mFireRate;

    uint64_t mLastFire;
};


//////////////////////////////////////////////////////////
//// Behaviors.
//////////////////////////////////////////////////////////

enum BehaviorType
{
	ApproachAndFire,
	Surround,
	DriveBy,
	Camp,
	Kamikaze,
    SpinningTop,
    PounceAndStalk
};

struct EnemyBehavior
{
	BehaviorType    mType;
    EnemyBehaviorCb mCb;
	uint8_t         mData[256];
};

struct ApproachAndFireBehaviorData
{
    float mSpeed;
    float mDistance;
};

struct SurroundBehaviorData
{
    float mSpeed;
    float mDistance;
};

struct DriveByBehaviorData
{
    float mSpeed;
    float mDistance;
    Vector2 mTurnPoint;
};

struct CampBehaviorData
{
    float mSpeed;
};

struct KamikazeBehaviorData
{
    float mSpeed;
    uint32_t mBiasUpdate;
    float mBias;
};

struct SpinningTopBehaviorData
{
    float mSpeed;
    float mRotation;
};

struct PounceAndStalkBehaviorData
{
    // Settings.
    float mSpeed;

    // Behavior btate tracking.
    enum State
    {
        RotateToMove=0,
        PreMoveRotate,
        CheckCollision,
        Move,
        RotateToJump,
        PreJumpRotate,
        Jump
    };

    State mState;
    uint64_t mJumpStart;
    float mDistance;  
    Vector2 mOrigin;
    bool mInCollision;
};


//////////////////////////////////////////////////////////
//// Particles.
//////////////////////////////////////////////////////////

enum EnemyParticleType
{
    ParticleNone,
    Smoke,
    JetFlame
};

struct EnemyParticle
{
    EnemyParticleType mType;
    char              mImagePath[256];
    EnemyParticleCb   mCb;
    uint8_t           mData[256];
};

struct EnemyParticleTemplate
{
    EnemyParticle     mParticle; 
    char              mImagePath[256];
};

struct SmokeParticleData
{
    Vector2 mPosition;
    float   mRotation;
    float   mSpeed;
};

struct JetFlameParticleData
{
    Vector2 mPosition;
    float   mRotation;
    float   mSpeed;
};

//////////////////////////////////////////////////////////
//// Images.
//////////////////////////////////////////////////////////

enum EnemyImageType
{
    Single,
    Flipbook
};


//////////////////////////////////////////////////////////
//// Enemy.
//////////////////////////////////////////////////////////

struct EnemyTemplate
{
    char                  mName[64];
    EnemyImageType        mImageType;
    char                  mImagePath[256];
    float                 mHealth;
    float                 mMass;
    float                 mRadius;

    uint32_t              mPointValue; // How many points is this enemy worth?
    uint32_t              mMinScore;   // What is the minimum player score to unlock this enemy?
    uint32_t              mMaxWaveCount; // Maximum number of this type of enemy per wave.

    EnemyBehavior         mBehavior;
    EnemyWeaponTemplate   mWeapons[ENEMY_MAX_WEAPON_COUNT];
    uint32_t              mWeaponCount;
    EnemyParticleTemplate mParticles[ENEMY_MAX_PARTICLE_COUNT];
    uint32_t              mParticleCount;
    
    Image*                mImages[ENEMY_MAX_IMAGE_COUNT];
    uint32_t              mImageCount;
};

struct Enemy
{
    ImageEntity          mEntity;
    Image*               mImages[ENEMY_MAX_IMAGE_COUNT];
    uint32_t             mImageCount;
    uint32_t             mCurrImage;
    uint32_t             mPointValue;

    Body*                mBody;
    VehicleController    mController;
    
	float                mHealth;

    EnemyBehavior        mBehavior;
	
    EnemyWeapon          mWeapon[ENEMY_MAX_WEAPON_COUNT];
    uint32_t             mWeaponCount;
    
    EnemyParticle        mParticle[ENEMY_MAX_PARTICLE_COUNT];
    uint32_t             mParticleCount;

	EnemyUpdateCb        mUpdateCb;
    EnemyDrawCb          mDrawCb;
	EnemyDamageCb        mDamageCb;

    // List maitenance.
    Enemy* mNext;
    Enemy* mPrev;
    Enemy* mRemoveNext;
};

struct SpawnEvent
{
    uint64_t mStartTime; // in usec from process start time.
    uint64_t mDuration;  // in usec.
    int32_t mCount;     // Count of how many to spawn.
    char     mEnemyName[256]; // Name of file, sans .csv
    uint32_t mSpawnId;   // Which spawn point?

    uint64_t mFreq;      // Calculated from def. in usec.
    uint64_t mLastSpawn; // in usec.
};

//////////////////////////////////////////////////////////
//// Manager.
//////////////////////////////////////////////////////////

class EnemyManager
    : public View
{
public:
    EnemyManager();
    virtual ~EnemyManager();
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;

    virtual void update();
    virtual void draw();

    void reset();

    void addSpawnPoint(Vector2 point);
    Vector2 const* getSpawnPoint(uint32_t index) const;
    uint32_t getSpawnPointCount() const;

    void addSpawnEvent(SpawnEvent& evt);

    // Loads the template for the named enemy.
    bool loadEnemyTemplate(char const* enemyName);

    // Initializes the passed enemy with named template.
    bool initializeEnemy(Enemy* e, char const* enemyName);
    bool initializeEnemy(Enemy* e, EnemyTemplate* enemyTemplate);

	Enemy* addEnemy();
    void removeEnemy(Enemy* e);

    uint32_t createWave(uint32_t pointValue);
    uint32_t mNextWaveScore;

protected:

private:


    typedef vector< Vector2 > Vector2Vector;
    Vector2Vector mSpawnPoints;

    typedef vector< SpawnEvent > SpawnEventVector;
    SpawnEventVector mSpawnEvents;

    typedef map< string, EnemyTemplate* > StringEnemyTemplateMap;
    StringEnemyTemplateMap mEnemyTemplates;

    typedef multimap< uint32_t, EnemyTemplate* > IntEnemyTemplateMap;
    IntEnemyTemplateMap mEnemyTemplatesByMinScore;


	Enemy* mEnemies;
    Enemy* mAddEnemies;
    Enemy* mRemoveEnemies;
    Enemy* mFreed;
};

} // namespace pammo

#endif //__ENEMYMANAGER_H__