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


void EnemyManager::addSpawnEvent(SpawnEvent& evt)
{
    uint64_t now = getTime();
    evt.mStartTime += now; // Change from 0 based to now based.
    if(evt.mCount == 0)
        return;

    evt.mFreq = evt.mDuration/evt.mCount;
    evt.mLastSpawn = 0;

    mSpawnEvents.push_back(evt);
}

void EnemyManager::update()
{
	while(mAddEnemies)
	{
		// Pop from add stack.
		Enemy* e = mAddEnemies;
		mAddEnemies = e->mNext;

        // Push onto active stack.
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

    uint32_t count=0;
    // Service spawn events.
    uint64_t now = getTime();
    for(SpawnEventVector::iterator i=mSpawnEvents.begin(); i!=mSpawnEvents.end(); ++i)
    {
        if((*i).mStartTime < now)
        {
            if(((*i).mLastSpawn + (*i).mFreq) <= now)
            {
                // spawn.
                Vector2 const* pos = getSpawnPoint((*i).mSpawnId);
                if(!pos)
                {
                    dprintf("Invalid spawn point id %d", (*i).mSpawnId);
                    assert(0);
                }

                Enemy* enemy = addEnemy();
			    enemy->mBody->mCenter = *pos;

                int type = (*i).mEnemyType;
                switch(type)
                {
                case 0:
				    trebuchetEnemyInit(enemy, this);
                    break;
                case 1:
                    sideShooterEnemyInit(enemy, this);
                    break;
                default:
                    dprintf("Invalid enemy type id %d", (*i).mEnemyType);
                    assert(0);
                }

                (*i).mLastSpawn = now;
                dprintf("%d Spawned type %d at point %d", count, (*i).mEnemyType, (*i).mSpawnId);
            }
        }

        ++count;
    }

    // Look for expired spawn events.
    SpawnEventVector::iterator i= mSpawnEvents.begin();
    while(i!=mSpawnEvents.end())
    {
        if(((*i).mStartTime + (*i).mDuration) < now)
        {
            // erase.
            i = mSpawnEvents.erase(i);
        }
        else
            ++i;
    }

    // Update enemies.
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

	return e;
}

void EnemyManager::removeEnemy(Enemy* e)
{
	// Push onto remove stack.
	e->mRemoveNext = mRemoveEnemies;
	mRemoveEnemies = e;
}

} // namespace pammo