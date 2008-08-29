#include "pammo.h"
#include "player.h"
#include "enemyManager.h"
#include "enemyLoader.h"
#include "world.h"
#include "physics.h"

#include "enemyWeaponFlamethrower.h"
#include "enemyWeaponTrebuchet.h"
#include "enemyWeaponSelfDestruct.h"
#include "enemyWeaponMachineGun.h"

#include "enemyParticleJetFlame.h"
#include "enemyParticleSmoke.h"

#include "camera.h"
#include "imageLibrary.h"
#include "minimap.h"


namespace pammo
{

    
float clamp(float v, float min, float max)
{
    float cmpMin = min;
    float cmpMax = max;

    if(v>cmpMin && cmpMin>cmpMax)
    {
        cmpMax+=(float)(M_PI*2.0f);
    }
    if(v>cmpMax)
        return max;

    if(v<cmpMin)
        return min;

    return v;
}


void enemyWeaponTurretUpdate(Enemy* e, EnemyWeapon* w, EnemyManager* manager, TurretWeaponData* data)
{
    // Calculate center. Vehicle center, plus turret offset rotated for vehicle, plus turret rotated for direction.
    data->mTurretCenter = e->mBody->mCenter
                     + data->mPosition
                     * Transform2::createRotation(e->mController.mRotation);

    Vector2 targetDirection = gWorld->getPlayer()->getCenter() - data->mTurretCenter;
    
    float rot;
    rot = atan2(targetDirection.y, targetDirection.x);

    // shift atan2 to 0-2PI
    rot+=(float)M_PI;

    // Remove vehicle rotation.
    rot-=e->mController.mRotation;
    if(rot > ((float)M_PI*2.0f))
        rot-= ((float)M_PI*2.0f);
    else if(rot < 0.0f)
        rot+= ((float)M_PI*2.0f);

    // Clamp local rotation.
    rot = clamp(rot, data->mRotationMin, data->mRotationMax);

    // Add back vehicle rotation.
    rot+= e->mController.mRotation;
    if(rot > ((float)M_PI*2.0f))
        rot-= ((float)M_PI*2.0f);
    else if(rot < 0.0f)
        rot+= ((float)M_PI*2.0f);

    data->mTurretRotation = rot+(float)M_PI;
    data->mTurretTip = data->mTurretCenter
                     + data->mFirePosition
                     * Transform2::createRotation(data->mTurretRotation);

    // Rotate 90 degrees to orient image.
    w->mEntity.mRotation = rot - (float)M_PI/2.0f;
    if(w->mEntity.mRotation < 0)
        w->mEntity.mRotation+= ((float)M_PI*2.0f);

    // Update weapon image entity.
    w->mEntity.mCenter = data->mTurretCenter;
    w->mEntity.makeDirty();
}

void enemyWeaponTurretGetParticleWithBody(Enemy* e, EnemyWeapon* w, EnemyManager* manager, TurretWeaponData* data, Particle** p)
{
    // Get a particle.
    *p = gWorld->getParticleSystem()->addParticleWithBody(2);
    if(!*p)
        return;

    // Set basic particle properties.
    (*p)->mAlpha = 1.0f;
    
    // Setup base image properties.
    (*p)->mImage.mCenter = data->mTurretTip;
    (*p)->mImage.mRotation = data->mTurretRotation;
    (*p)->mImage.makeDirty();
        
    // Base properties about particle bodies.
    (*p)->mBody->mProperties = kPlayerBulletCollisionProperties;
    (*p)->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    (*p)->mBody->mCenter = data->mTurretTip;
}



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

    e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
    e->mController.mRotationTarget = rot;
    e->mEntity.makeDirty();
}

void behaviorApproachAndFireCb(Enemy* e, EnemyManager* manager)
{
    ApproachAndFireBehaviorData* data = (ApproachAndFireBehaviorData*)e->mBehavior.mData;

    e->mController.update();

    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float mag = magnitude(heading);
    float speed = .5f;
    float rot = atan2(heading.y, heading.x);
    if(rot < 0) 
        rot += (float)M_PI*2;
    e->mController.mRotationTarget = rot;

    if(mag < data->mDistance)
    {
        e->mController.mAcceleration = 0;
    }
    else
    {
        e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
    }

    e->mEntity.makeDirty();
}

void behaviorDriveByCb(Enemy* e, EnemyManager* manager)
{}

void behaviorCampCb(Enemy* e, EnemyManager* manager)
{}

void behaviorKamikazeCb(Enemy* e, EnemyManager* manager)
{
    ApproachAndFireBehaviorData* data = (ApproachAndFireBehaviorData*)e->mBehavior.mData;

    e->mController.update();

    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float rot = atan2(heading.y, heading.x);
    if(rot < 0) 
        rot += (float)M_PI*2;
    e->mController.mRotationTarget = rot;
    e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
}

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

    // Update particles.
    for(uint32_t i=0; i<e->mParticleCount; ++i)
    {
        e->mParticle[i].mCb(e, &e->mParticle[i], manager);
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
		gWorld->getParticleSystem()->initRubbleParticle(e->mBody->mCenter);
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
    e->mBody->mBodyCallback = NULL;
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
    e->mDrawCb   = enemyDrawCb;
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
            e->mWeapon[i].mCb = enemyWeaponFlamethrowerCb;
            break;
        case MachineGun:
            e->mWeapon[i].mCb = enemyWeaponMachineGunCb;
            break;
        case Trebuchet:
            e->mWeapon[i].mCb = enemyWeaponTrebuchetCb;
            break;
        case SelfDestruct:
            e->mWeapon[i].mCb = enemyWeaponSelfDestructCb;
            e->mBody->mBodyCallback = enemyWeaponSelfDestructCollisionCb;
            break;

        }
    }    
    
    // Particles.
    e->mParticleCount = enemyTemplate->mParticleCount;
    for(uint32_t i=0; i<e->mParticleCount; ++i)
    {
        memcpy(&e->mParticle[i], &enemyTemplate->mParticles[i].mParticle, sizeof(EnemyParticle));
        strcpy(e->mParticle[i].mImagePath, enemyTemplate->mParticles[i].mImagePath);
        switch(e->mParticle[i].mType)
        {
        case Smoke:
            e->mParticle[i].mCb = enemyParticleSmokeCb;
            break;
        case JetFlame:
            e->mParticle[i].mCb = enemyParticleJetFlameCb;
            break;
        };
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
        if(((*i).mStartTime + (*i).mDuration) <= now)
        {
            // erase.
            dprintf("Deleting expired spawn event");
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