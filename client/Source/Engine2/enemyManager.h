#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "pammo.h"
#include "view.h"

#include "vehicleController.h"
#include "imageEntity.h"
#include "particleSystem.h"
#include <vector>

#define ENEMY_DATA_SIZE 128
#define ENEMY_COUNT
namespace pammo
{

class EnemyManager;
struct Enemy;

typedef void (*EnemyUpdateCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDrawCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDamageCb)(Enemy* e, ParticleType type, float amount);
typedef void (*EnemyDestroyCb)(Enemy* e, EnemyManager* manager);

/*enum Weapon
{
	Flamethrower,
	LightningGun,
	Catapult,
	MachineGun
};*/

struct EnemyWeapon
{
//	Weapon mType;
    float  mMinRange;
    float  mMaxRange;
    float  mFireFrequency; // 0.0 - 1.0
};

enum Behavior
{
	ApproachAndFire,
	Surround,
	DriveBy,
	HideAndCamp,
	Kamakazi
};

struct EnemyBehavior
{
	Behavior mType;
	uint8_t mData[256];
};

struct Enemy
{
    ImageEntity*         mEntity;
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

//    uint8_t mData[ENEMY_DATA_SIZE];
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
    typedef vector< Vector2 > Vector2Vector;
    Vector2Vector mSpawnPoints;

    typedef vector< SpawnEvent > SpawnEventVector;
    SpawnEventVector mSpawnEvents;

	Enemy* mEnemies;
    Enemy* mAddEnemies;
    Enemy* mRemoveEnemies;
    Enemy* mFreed;

//	uint32_t mActiveEnemyCount;
//	uint64_t mLastEnemy;
};

} // namespace pammo

#endif //__ENEMYMANAGER_H__