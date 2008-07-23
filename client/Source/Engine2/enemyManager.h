#ifndef __ENEMYMANAGER_H__
#define __ENEMYMANAGER_H__

#include "pammo.h"
#include "view.h"

#include "vehicleController.h"
#include "imageEntity.h"

#include <vector>

#define ENEMY_DATA_SIZE 128
#define ENEMY_COUNT
namespace pammo
{

class EnemyManager;
struct Enemy;

typedef void(*EnemyUpdateCb)(Enemy* e, EnemyManager* manager);
typedef void (*EnemyDrawCb)(Enemy* e, EnemyManager* manager);

struct Enemy
{
    ImageEntity*         mEntity;
    Body*                mBody;
    VehicleController*   mController;
    EnemyUpdateCb        mUpdateCb;
    EnemyDrawCb          mDrawCb;

    uint8_t mData[ENEMY_DATA_SIZE];
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

    enum EnemyType
    {
        TrebuchetEnemy,
        SideShooterEnemy
    };

protected:
    Enemy* createEnemy(EnemyType type, Vector2 const& position);
    void destroyEnemy(Enemy* e);

private:
    typedef vector< Vector2 > Vector2Vector;
    Vector2Vector mSpawnPoints;

    typedef vector< Enemy* > EnemyVector; 
    EnemyVector mEnemies;
    uint64_t mLastEnemy;
};

} // namespace pammo

#endif //__ENEMYMANAGER_H__