#include "pammo.h"
#include "player.h"
#include "enemyManager.h"
#include "enemyLoader.h"
#include "world.h"
#include "physics.h"
#include "trebuchetEnemy.h"
#include "flameTankEnemy.h"
#include "sideShooterEnemy.h"
#include "camera.h"
#include "imageLibrary.h"
#include "minimap.h"

float clamp(float v,float min, float max)
{
    if(v<min)
        return min;
    if(v>max)
        return max;
    return v;
}

namespace pammo
{
void behaviorSurroundCb(Enemy* e, EnemyManager* manager)
{
    SurroundBehaviorData* data = (SurroundBehaviorData*)e->mBehavior.mData;

    // Find vector to player.
    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float mag = magnitude(heading);
    float rot = atan2(heading.y, heading.x);

    // within target distance.
    if(mag < data->mDistance)
    {
        // turn.
        rot+=(1.57f/1.5f);
    }
    if(rot < 0) 
        rot+= (float)M_PI*2.0f;
    else if(rot > (float)M_PI*2.0f)
        rot-= (float)M_PI*2.0f;

    e->mController.mAcceleration = data->mSpeed * e->mBody->mMass; // SCD magic 7?
    e->mController.mRotationTarget = rot;
    e->mEntity.makeDirty();
}

void behaviorApproachAndFireCb(Enemy* e, EnemyManager* manager)
{}

void behaviorDriveByCb(Enemy* e, EnemyManager* manager)
{}

void behaviorCampCb(Enemy* e, EnemyManager* manager)
{}

void behaviorKamikazeCb(Enemy* e, EnemyManager* manager)
{}

//// Flamethrower /////


void weaponFlamethrowerFireParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= 1.05f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.085f;
}

void weaponFlamethrowerFireCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

	doDamage(self, other, Fire, 10.0f);
}

void weaponFlamethrowerCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{

    FlamethrowerWeaponData* data = (FlamethrowerWeaponData*)w->mData;

    Vector2 targetDirection = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float rot = atan2(targetDirection.y, targetDirection.x);

    //dprintf("%.2f %.2f    %.2f", targetDirection.x, targetDirection.y, targetRot);
    
    rot = clamp(rot, data->mTurret.mRotationMin-M_PI, data->mTurret.mRotationMax+M_PI);

    if(rot > ((float)M_PI*2.0f))
        rot-= ((float)M_PI*2.0f);
    //else if(rot < 0)
    //    rot+= (float)M_PI*2.0f;

    // Calculate center. Vehicle center plus arm rotated for direction.
    Vector2 center = e->mBody->mCenter
                     + data->mTurret.mPosition
                     + data->mTurret.mFirePosition
                     * Transform2::createRotation(rot);

    // Update weapon image entity.
    w->mEntity.mCenter = e->mBody->mCenter
                     + data->mTurret.mPosition
                     * Transform2::createRotation(rot+ (float)M_PI/2.0f);
    w->mEntity.mRotation = rot + (float)M_PI/2;
    w->mEntity.makeDirty();
    return;
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2);
    if(!p)
        return;

    // Set basic particle properties.
    p->mCallback = weaponFlamethrowerFireParticleCb;
    p->mAlpha = 1.0f;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.mCenter = center;
    p->mImage.mRotation = rot;
    p->mImage.makeDirty();
        
    // Properties about fire particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mBodyCallback = weaponFlamethrowerFireCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 1;
    p->mBody->mCenter = center;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(rot);
}


//// Trebuchet ////

void weaponTrebuchetCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{}
void weaponMachineGunCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{}

void enemyUpdateCb(Enemy* e, EnemyManager* manager)
{
    e->mController.update();

    // Update behavior.
    e->mBehavior.mCb(e, manager);

    // Update weapons.
    for(uint32_t i=0; i<e->mWeaponCount; ++i)
    {
        e->mWeapon[i].mCb(e, &e->mWeapon[i], manager);
    }

    // Update image entity.
    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();
}

void enemyDrawCb(Enemy* e, EnemyManager* manager)
{
    e->mEntity.draw();
    for(uint32_t i=0; i<e->mWeaponCount; ++i)
    {
        e->mWeapon[i].mEntity.draw();
    }
}

void enemyDamageCb(Enemy* e, ParticleType type, float amount)
{
	e->mHealth-=amount;

	if(e->mHealth <=0)
	{
		dprintf("dead", e);
		gWorld->getParticleSystem()->initExplosionParticle(e->mBody->mCenter);
		gImageLibrary->unreference(e->mEntity.getImage());
        for(uint32_t i=0; i<e->mWeaponCount; ++i)
        {
            gImageLibrary->unreference(e->mWeapon[i].mEntity.getImage());
        }
		
        
        gWorld->getEnemyManager()->removeEnemy(e);
		e->mDamageCb = NULL;
	}
}





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

bool EnemyManager::loadEnemyTemplate(char const* enemyName)
{
    EnemyTemplate* enemyTemplate = new EnemyTemplate;
    EnemyLoader loader;
    char path[256];
    snprintf(path, 255, "data/enemies/%s.csv", enemyName);
    if(loader.load(path, enemyTemplate) == false)
    {
        delete enemyTemplate;
        return false;
    }

    mEnemyTemplates[std::string(enemyName)] = enemyTemplate;
    return true;
}


bool EnemyManager::initializeEnemy(Enemy* e, char const* name)
{
    StringEnemyTemplateMap::iterator i = mEnemyTemplates.find(name);
    if(i == mEnemyTemplates.end())
    {
        if(loadEnemyTemplate(name) == false)
            return false;
        i = mEnemyTemplates.find(name);
        if(i == mEnemyTemplates.end())
            return false;
    }

    EnemyTemplate* enemyTemplate = i->second;

    // Properties.
    e->mBody->mMass =   enemyTemplate->mMass;
    e->mBody->mRadius = enemyTemplate->mRadius;
    e->mHealth =        enemyTemplate->mHealth;
    if(enemyTemplate->mImageType == Single)
    {
        e->mEntity.setImageAndInit(gImageLibrary->reference(enemyTemplate->mImagePath));
    }    
    else
    {
        dprintf("Only single images are currenly supported.");
        assert(0);
    }

    e->mUpdateCb = enemyUpdateCb;
    e->mDrawCb = enemyDrawCb;
    e->mDamageCb = enemyDamageCb;

    // Behavior.
    memcpy(&e->mBehavior, &enemyTemplate->mBehavior, sizeof(EnemyBehavior));
    switch(e->mBehavior.mType)
    {
    case ApproachAndFire:
        e->mBehavior.mCb = behaviorApproachAndFireCb;
        break;
    case Surround:
        e->mBehavior.mCb = behaviorSurroundCb;
        break;
    case DriveBy:
        e->mBehavior.mCb = behaviorDriveByCb;
        break;
    case Camp:
        e->mBehavior.mCb = behaviorCampCb;
        break;
    case Kamikaze:
        e->mBehavior.mCb = behaviorKamikazeCb;
        break;
    }
    
    // Weapons.
    e->mWeaponCount = enemyTemplate->mWeaponCount;
    for(uint32_t i=0; i<e->mWeaponCount; ++i)
    {
        memcpy(&e->mWeapon[i], &enemyTemplate->mWeapons[i].mWeapon, sizeof(EnemyWeapon));
        e->mWeapon[i].mEntity.setImageAndInit(gImageLibrary->reference(enemyTemplate->mWeapons[i].mImagePath));
        switch(e->mWeapon[i].mType)
        {
        case Flamethrower:
            e->mWeapon[i].mCb = weaponFlamethrowerCb;
            break;
        case MachineGun:
            e->mWeapon[i].mCb = weaponMachineGunCb;
            break;
        case Trebuchet:
            e->mWeapon[i].mCb = weaponTrebuchetCb;
            break;
        }
    }    
    
    
    return true;
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
                if(initializeEnemy(enemy, (*i).mEnemyName) == false)
                {
                    dprintf("Failed to initialize enemy %s.");
                    assert(0);
                }
                enemy->mBody->mCenter = *pos;

                (*i).mLastSpawn = now;
                dprintf("Spawned type %s at point %d", (*i).mEnemyName, (*i).mSpawnId);
            }
        }
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
    Minimap* minimap = gWorld->getMinimap();
	while(e)
	{
		e->mUpdateCb(e, this);
        minimap->markEnemy(e->mBody->mCenter);
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
    e->mBody->mCollideProperties= kEnemyCollisionProperties | kPlayerCollisionProperties | kBarrierCollisionProperties;
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
    gWorld->getPhysics()->removeBody(e->mBody);
    // Push onto remove stack.
	e->mRemoveNext = mRemoveEnemies;
	mRemoveEnemies = e;
}

} // namespace pammo