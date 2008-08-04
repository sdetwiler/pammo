#include "pammo.h"
#include "enemyManager.h"
#include "world.h"
#include "physics.h"
#include "trebuchetEnemy.h"
#include "sideShooterEnemy.h"
#include "camera.h"

#include <algorithm>

namespace pammo
{

EnemyManager::EnemyManager()
{
    mLastEnemy = 0;
	mActiveEnemyCount = 0;
	mAddEnemies = NULL;
	mRemoveEnemies = NULL;
	mEnemies = NULL;
	mFreed = NULL;
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
	while(mAddEnemies)
	{
		// Pop from add stack.
		Enemy* e = mAddEnemies;
		mAddEnemies = e->mNext;

        e->mNext = mEnemies;
        if(e->mNext)
			e->mNext->mPrev = e;
        mEnemies = e;
	}

	while(mRemoveEnemies)
	{
		// Pop from remove stack.
		Enemy* e = mRemoveEnemies;
		mRemoveEnemies = e->mRemoveNext;

		// Remove from active list.
		if(e->mNext)
			e->mNext->mPrev = e->mPrev;
		if(e->mPrev)
			e->mPrev->mNext = e->mNext;
		else
			mEnemies = e->mNext;

		// Push onto avail stack.
		e->mPrev = NULL;
		e->mNext = mFreed;
		mFreed = e;
	}


    if(mActiveEnemyCount < 50 && 0)
    {
        uint64_t now = getTime();
        if(now-mLastEnemy > 2500000)
        {
            Vector2 const* pos = getSpawnPoint(rand()%getSpawnPointCount());
            Enemy* enemy = addEnemy();
			enemy->mBody->mCenter = *pos;

			int type = 0;//rand()%2;
            switch(type)
            {
            case 0:
				trebuchetEnemyInit(enemy, this);
                break;
            case 1:
                sideShooterEnemyInit(enemy, this);
                break;
            }

			mLastEnemy = now;
        }
    }

	Enemy* e = mEnemies;
	while(e)
	{
		e->mUpdateCb(e, this);
		assert(e!=e->mNext);
		e = e->mNext;
	}
}

void EnemyManager::draw()
{
    gWorld->getCamera()->set();

	Enemy* e = mEnemies;
	while(e)
	{
		e->mDrawCb(e, this);
		e = e->mNext;
	}
    gWorld->getCamera()->unset();
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


Enemy* EnemyManager::addEnemy()
{
    Enemy* e;
    if(mFreed)
    {
        e = mFreed;
        mFreed = e->mNext;
		memset(e, 0, sizeof(e));
    }
    else
    {
        e = new Enemy();
		memset(e, 0, sizeof(e));
    }

	e->mNext = mAddEnemies;
	e->mPrev = NULL;
	mAddEnemies = e;

    e->mBody = gWorld->getPhysics()->addBody();
    e->mBody->mProperties = kEnemyCollisionProperties;
    e->mBody->mCollideProperties= kEnemyCollisionProperties | kPlayerCollisionProperties;
    e->mBody->mDamping = 0.1f;
    e->mBody->mRadius = 20;
    e->mBody->mMass = 100;
	e->mBody->mUserArg = e;
    e->mController.mBody = e->mBody;
    e->mController.mRotationDamping = 0.4f;

	++mActiveEnemyCount;
	return e;
}

void EnemyManager::removeEnemy(Enemy* e)
{
	// Push onto remove stack.
	e->mRemoveNext = mRemoveEnemies;
	mRemoveEnemies = e;

	--mActiveEnemyCount;
}

} // namespace pammo