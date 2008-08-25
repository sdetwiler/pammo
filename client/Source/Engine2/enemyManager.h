#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "pammo.h"
#include "view.h"

#include "vehicleController.h"
#include "imageEntity.h"
#include "particleSystem.h"
#include <vector>

#define ENEMY_MAX_WEAPON_COUNT 4
//#define ENEMY_DATA_SIZE 128
//#define ENEMY_COUNT
namespace pammo
{

class EnemyManager;
struct Enemy;
struct EnemyWeapon;
struct TurretWeaponData;

typedef void (*EnemyBehaviorCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyWeaponCb)(Enemy* e, EnemyWeapon* w, EnemyManager* manager);

typedef void (*EnemyUpdateCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDrawCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDamageCb)(Enemy* e, ParticleType type, float amount);
typedef void (*EnemyDestroyCb)(Enemy* e, EnemyManager* manager);

float clamp(float v, float min, float max);
void enemyWeaponTurretGetParticleWithBody(Enemy* e, EnemyWeapon* w, EnemyManager* manager, TurretWeaponData* data, Particle** p);

enum WeaponType
{
    None,
    Flamethrower,
    MachineGun,
    Trebuchet
};

struct TurretWeaponData
{
    Vector2        mPosition;
    Vector2        mFirePosition;
    float          mRotationMin;
    float          mRotationMax;
    float          mRotationCur;
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
    uint32_t mFireRate;
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

enum BehaviorType
{
	ApproachAndFire,
	Surround,
	DriveBy,
	Camp,
	Kamikaze
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
};

struct CampBehaviorData
{
    float mSpeed;
};

struct KamikazeBehaviorData
{
    float mSpeed;
};

struct EnemyBehavior
{
	BehaviorType    mType;
    EnemyBehaviorCb mCb;
	uint8_t         mData[256];
};

enum EnemyImageType
{
    Single,
    Flipbook
};

struct EnemyTemplate
{
    char                mName[64];
    EnemyImageType      mImageType;
    char                mImagePath[256];
    float               mHealth;
    float               mMass;
    float               mRadius;
    EnemyBehavior       mBehavior;
    EnemyWeaponTemplate mWeapons[ENEMY_MAX_WEAPON_COUNT];
    uint32_t            mWeaponCount;
};

struct Enemy
{
    ImageEntity          mEntity;
    Body*                mBody;
    VehicleController    mController;
    
	float                mHealth;

	EnemyWeapon          mWeapon[ENEMY_MAX_WEAPON_COUNT];
    uint32_t             mWeaponCount;
    EnemyBehavior        mBehavior;

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
    uint32_t mCount;     // Count of how many to spawn.
    char     mEnemyName[256]; // Name of file, sans .csv
    uint32_t mSpawnId;   // Which spawn point?

    uint64_t mFreq;      // Calculated from def. in usec.
    uint64_t mLastSpawn; // in usec.
};

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

    void addSpawnPoint(Vector2 point);
    Vector2 const* getSpawnPoint(uint32_t index) const;
    uint32_t getSpawnPointCount() const;

    void addSpawnEvent(SpawnEvent& evt);

    // Initializes the passed enemy with named template.
    bool initializeEnemy(Enemy* e, char const* enemyName);

	Enemy* addEnemy();
    void removeEnemy(Enemy* e);

    enum EnemyType
    {
        TrebuchetEnemy,
        FlameTankEnemy,
        SideShooterEnemy
    };

protected:

private:

    // Loads the template for the named enemy.
    bool loadEnemyTemplate(char const* enemyName);

    typedef vector< Vector2 > Vector2Vector;
    Vector2Vector mSpawnPoints;

    typedef vector< SpawnEvent > SpawnEventVector;
    SpawnEventVector mSpawnEvents;

    typedef map< string, EnemyTemplate* > StringEnemyTemplateMap;
    StringEnemyTemplateMap mEnemyTemplates;

	Enemy* mEnemies;
    Enemy* mAddEnemies;
    Enemy* mRemoveEnemies;
    Enemy* mFreed;
};

} // namespace pammo

#endif //__ENEMYMANAGER_H__