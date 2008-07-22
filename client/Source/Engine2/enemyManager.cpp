#include "pammo.h"
#include "enemyManager.h"
#include "trebuchetEnemy.h"
#include "sideShooterEnemy.h"

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
        if(now-mLastEnemy > 5000000)
        {

            Vector2 const* pos = getSpawnPoint(rand()%getSpawnPointCount());
            Enemy* enemy;
            int type = rand()%2;
            switch(type)
            {
            case 0:
                enemy = new TrebuchetEnemy(*pos);
                break;
            case 1:
                enemy = new SideShooterEnemy(*pos);
                break;
            }
            
            mEnemies.push_back(enemy);
            mLastEnemy = now;
        }
    }

    for(EnemyVector::iterator i = mEnemies.begin(); i!= mEnemies.end(); ++i)
    {
        (*i)->update();
    }
}

void EnemyManager::draw()
{
    for(EnemyVector::iterator i = mEnemies.begin(); i!= mEnemies.end(); ++i)
    {
        (*i)->draw();
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
    return mSpawnPoints.size();
}


} // namespace pammo