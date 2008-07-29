#include "pammo.h"
#include "enemyManager.h"
#include "world.h"
#include "physics.h"
#include "trebuchetEnemy.h"
#include "sideShooterEnemy.h"

#include <algorithm>

namespace pammo
{

EnemyManager::EnemyManager()
{
    mLastEnemy = 0;
}

EnemyManager::~EnemyManager()
{

}

uint32_t EnemyManager::getUpdatePriority() const
{
    return kEnemyManagerPriority;
}

uint32_t EnemyManager::getDrawPriority() const
{
    return kEnemyManagerPriority;
}

void EnemyManager::update()
{
    if(mEnemies.size() < 50)
    {
        uint64_t now = getTime();
        if(now-mLastEnemy > 2500000)
        {
            Vector2 const* pos = getSpawnPoint(rand()%getSpawnPointCount());
            Enemy* enemy;
            int type = 0;//rand()%2;
            switch(type)
            {
            case 0:
                enemy = createEnemy(TrebuchetEnemy, *pos);
                break;
            case 1:
                enemy = createEnemy(SideShooterEnemy, *pos);
                break;
            }
            
            mEnemies.push_back(enemy);
            mLastEnemy = now;
        }
    }

    for(EnemyVector::iterator i = mEnemies.begin(); i!= mEnemies.end(); ++i)
    {
        (*i)->mUpdateCb((*i), this);
    }
}

void EnemyManager::draw()
{
    for(EnemyVector::iterator i = mEnemies.begin(); i!= mEnemies.end(); ++i)
    {
        (*i)->mDrawCb((*i), this);
    }
}

void EnemyManager::addSpawnPoint(Vector2 point)
{
    mSpawnPoints.push_back(point);
}

Vector2 const* EnemyManager::getSpawnPoint(uint32_t index) const
{
    if(index > getSpawnPointCount())
        return NULL;

    return &mSpawnPoints[index];
}

uint32_t EnemyManager::getSpawnPointCount() const
{
    return (uint32_t)mSpawnPoints.size();
}

Enemy* EnemyManager::createEnemy(EnemyType type, Vector2 const& position)
{
    // Should pull from pool.
    Enemy* e = new Enemy;
    memset(e, 0, sizeof(Enemy));

    e->mBody = gWorld->getPhysics()->addBody();
    e->mBody->mCenter = position;
    e->mBody->mProperties = kEnemyCollisionProperties;
    e->mBody->mCollideProperties= kEnemyCollisionProperties | kPlayerCollisionProperties;
    e->mBody->mDamping = 0.1f;
    e->mBody->mRadius = 20;
    e->mBody->mMass = 100;
	e->mBody->mUserArg = e;
    e->mController = new VehicleController();
    e->mController->mBody = e->mBody;
    e->mController->mRotationDamping = 0.4f;
	e->mDrawCb = NULL;
	e->mUpdateCb = NULL;
	e->mDamageCb = NULL;
	e->mDestroyCb = NULL;
	e->mDestroyed = false;

    switch(type)
    {
    case TrebuchetEnemy:
        trebuchetEnemyInit(e, this);
        break;
    case SideShooterEnemy:
        sideShooterEnemyInit(e, this);
        break;
    }

    return e;
}

void EnemyManager::destroyEnemy(Enemy* e)
{
	if(e->mDestroyed == true)
		return;

	e->mDestroyed = true;

	e->mDestroyCb(e, this);
	mEnemies.erase(find(mEnemies.begin(), mEnemies.end(), e));
    gWorld->getPhysics()->removeBody(e->mBody);
    // Should return to pool.
    delete e;
}

} // namespace pammo