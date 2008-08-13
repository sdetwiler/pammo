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

typedef void (*EnemyUpdateCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDrawCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDamageCb)(Enemy* e, ParticleType type, float amount);
typedef void (*EnemyDestroyCb)(Enemy* e, EnemyManager* manager);


enum WeaponType
{
    None,
    Flamethrower,
    MachineGun,
    Trebuchet
};

struct FlamethrowerWeaponData
{
    int32_t mPositionX;
    int32_t mPositionY;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mRotationMin;
    uint32_t mRotationMax;
    uint32_t mMaxDistance;
    uint32_t mSpreadAngle;
};

struct TrebuchetWeaponData
{
    int32_t mPositionX;
    int32_t mPositionY;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mRotationMin;
    uint32_t mRotationMax;
    uint32_t mMaxDistance;
    uint32_t mFireRate;
};

struct MachineGunWeaponData
{
    int32_t mPositionX;
    int32_t mPositionY;
    uint32_t mAccuracy;
    uint32_t mDamage;
    uint32_t mRotationMin;
    uint32_t mRotationMax;
    uint32_t mMaxDistance;
    uint32_t mFireRate;
};

struct EnemyWeapon
{
	WeaponType mType;
    uint8_t    mData[256];
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
	BehaviorType mType;
	uint8_t      mData[256];
};

enum EnemyImageType
{
    Single,
    Flipbook
};

struct EnemyTemplate
{
    char            mName[64];
    EnemyImageType  mImageType;
    char            mImagePath[256];
    float           mHealth;
    float           mMass;
    float           mRadius;
    EnemyBehavior   mBehavior;
    EnemyWeapon     mWeapons[ENEMY_MAX_WEAPON_COUNT];
    uint32_t        mWeaponCount;
};

struct Enemy
{
    ImageEntity          mEntity;
    Body*                mBody;
    VehicleController    mController;
    
	float                mHealth;

	EnemyWeapon          mWeapon;
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
    uint32_t mEnemyType; // 0: trebuchet, 1: flametank
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