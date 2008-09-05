#include "pammo.h"
#include "player.h"
#include "enemyManager.h"
#include "enemyLoader.h"
#include "world.h"
#include "physics.h"
#include "flipbookLoader.h"

#include "enemyWeaponFlamethrower.h"
#include "enemyWeaponTrebuchet.h"
#include "enemyWeaponSelfDestruct.h"
#include "enemyWeaponMachineGun.h"
#include "enemyWeaponHeatSeaker.h"
#include "enemyWeaponMineLayer.h"

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
    (*p)->mBody->mCollideProperties = kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
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
    e->mCurrImage = (e->mCurrImage+1) % e->mImageCount;
    e->mEntity.setImage(e->mImages[e->mCurrImage]);
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
        gWorld->getPlayer()->mScore += e->mPointValue;

        for(uint32_t i=0; i<e->mImageCount; ++i)
        {
            gImageLibrary->unreference(e->mImages[i]);
        }

        //gImageLibrary->unreference(e->mEntity.getImage());

        for(uint32_t i=0; i<e->mWeaponCount; ++i)
        {
            gImageLibrary->unreference(e->mWeapon[i].mEntity.getImage());
        }
        
        gWorld->getEnemyManager()->removeEnemy(e);
		e->mDamageCb = NULL;
        e->mUpdateCb = NULL;
	}
}



EnemyManager::EnemyManager()
{
    mNextWaveScore = 0;
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
    if(evt.mCount == 0)
        return;

    uint64_t now = getTime();
    evt.mStartTime += now; // Change from 0 based to now based.

    evt.mFreq = evt.mDuration/evt.mCount;
    evt.mLastSpawn = 0;

    mSpawnEvents.push_back(evt);
}

bool EnemyManager::loadEnemyTemplate(char const* enemyName)
{
    EnemyTemplate* enemyTemplate = new EnemyTemplate;
    
    enemyTemplate->mMinScore = 0;
    enemyTemplate->mPointValue = 0;

    EnemyLoader loader;
    char path[256];
    snprintf(path, 255, "data/enemies/%s.csv", enemyName);
    if(loader.load(path, enemyTemplate) == false)
    {
        delete enemyTemplate;
        return false;
    }

    if(enemyTemplate->mImageType == Single)
    {
        enemyTemplate->mImages[0] = gImageLibrary->reference(enemyTemplate->mImagePath);
        enemyTemplate->mImageCount = 1;
    }    
    else
    {
        loadFlipbook(enemyTemplate->mImagePath, enemyTemplate->mImages, ENEMY_MAX_IMAGE_COUNT, &enemyTemplate->mImageCount);

        //DIR* dir;
        //dir = opendir(enemyTemplate->mImagePath);
        //if(!dir)
        //{
        //    dprintf("Failed to open flipbook directory: %s", enemyTemplate->mImagePath);
        //    assert(0);
        //    return false;
        //}

        //typedef std::set< std::string > StringSet;
        //StringSet filenames;
        //struct dirent* item;

        //while((item = readdir(dir)) != NULL)
        //{
        //    int len = strlen(item->d_name);
        //    if(!strcmp(&(item->d_name[len-3]), "png"))
        //    {
        //        filenames.insert(item->d_name);
        //    }
        //}

        //closedir(dir);

        //int i=0;
        //for(StringSet::iterator j = filenames.begin(); j!=filenames.end() && i<ENEMY_MAX_IMAGE_COUNT; ++i, ++j)
        //{
        //    char filename[256];
        //    snprintf(filename, 255, "%s/%s", enemyTemplate->mImagePath, (*j).c_str());
        //    enemyTemplate->mImages[i] = gImageLibrary->reference(filename);
        //    if(!enemyTemplate->mImages[i])
        //    {
        //        dprintf("Failed to load image %s", filename);
        //        assert(0);
        //        return false;
        //    }

        //    ++enemyTemplate->mImageCount;
        //}

        //dprintf("Flipbook loaded %u images. ImageCount is %u", i, enemyTemplate->mImageCount);
    }

    mEnemyTemplates[std::string(enemyName)] = enemyTemplate;
    mEnemyTemplatesByMinScore.insert(IntEnemyTemplateMap::value_type(enemyTemplate->mMinScore, enemyTemplate));
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
    return initializeEnemy(e, enemyTemplate);
}

bool EnemyManager::initializeEnemy(Enemy* e, EnemyTemplate* enemyTemplate)
{
    // Properties.
    e->mBody->mBodyCallback = NULL;
    e->mBody->mMass =   enemyTemplate->mMass;
    e->mBody->mRadius = enemyTemplate->mRadius;

    e->mHealth =        enemyTemplate->mHealth;
    e->mPointValue =    enemyTemplate->mPointValue;

    e->mEntity.mAlpha = 1.0f;
    e->mEntity.mCenter = Vector2();
    e->mEntity.mRotation = 0;

    e->mCurrImage = 0;
    e->mImageCount = enemyTemplate->mImageCount;
    for(uint32_t i=0; i<e->mImageCount; ++i)
    {
        e->mImages[i] = enemyTemplate->mImages[i];
        gImageLibrary->reference(e->mImages[i]);
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
        case HeatSeaker:
            e->mWeapon[i].mCb = enemyWeaponHeatSeakerCb;
            break;
        case Trebuchet:
            e->mWeapon[i].mCb = enemyWeaponTrebuchetCb;
            break;
        case SelfDestruct:
            e->mWeapon[i].mCb = enemyWeaponSelfDestructCb;
            e->mBody->mBodyCallback = enemyWeaponSelfDestructCollisionCb;
            break;
        case MineLayer:
            e->mWeapon[i].mCb = enemyWeaponMineLayerCb;
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
        if((*i).mStartTime <= now)
        {
            if(((*i).mLastSpawn + (*i).mFreq) <= now)
            {
                --(*i).mCount;
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
        if((*i).mCount == 0)
        {
            // erase.
            dprintf("Deleting expired spawn event. Count is %u", (*i).mCount);
            i = mSpawnEvents.erase(i);
        }
        else
            ++i;
    }

    // Check if a new wave should be created.
    if(gWorld->getPlayer()->mScore >= mNextWaveScore)
    {
        uint32_t wavePoints = gWorld->getPlayer()->mScore;
        if(!wavePoints)
            wavePoints = 50;
        mNextWaveScore+= createWave(wavePoints);
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
    dprintf("Add spawn point: %u  <%.2f, %.2f>", mSpawnPoints.size(), point.x, point.y);
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
    e->mBody->mCollideProperties= kEnemyCollisionProperties | kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
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

uint32_t EnemyManager::createWave(uint32_t pointValue)
{
    uint32_t pointsAdded = 0;
    dprintf("createWave %u points", pointValue);

    struct EnemyTemplateCount
    {
        EnemyTemplateCount()
        {
            mEnemyTemplate = NULL;
            mCount = 0;
        }

        EnemyTemplateCount(EnemyTemplate* t)
        {
            mEnemyTemplate = t;
            mCount = 0;
        }
        uint32_t mCount;
        EnemyTemplate* mEnemyTemplate;
    };

    // Find all enemy templates that should be unlocked.
    typedef vector< EnemyTemplateCount > EnemyTemplateCountVector;
    EnemyTemplateCountVector templates;
    for(IntEnemyTemplateMap::iterator i = mEnemyTemplatesByMinScore.begin(); i!=mEnemyTemplatesByMinScore.end(); ++i)
    {
        if(i->first > gWorld->getPlayer()->mScore)
            break;

        templates.push_back(EnemyTemplateCount(i->second));
    }

    if(templates.size() == 0)
    {
        dprintf("createWave failed. No available enemy templates");
        return 0xffffffff;
    }

    //Vector2 const* pos = getSpawnPoint(rand() % getSpawnPointCount());

    int noOpPassCount = 0;
    // While there are points left to distribute for this wave.
    uint32_t pointsRemain = pointValue;
    while(pointsRemain && noOpPassCount<3 && templates.size())
    {
        ++noOpPassCount;

        // Pick a random unlocked enemy and create a spawn event sometime in the near future.
        EnemyTemplateCount& enemyTemplateCount = templates[rand()%templates.size()];
        SpawnEvent spawnEvent;
        strcpy(spawnEvent.mEnemyName, enemyTemplateCount.mEnemyTemplate->mName);
        spawnEvent.mStartTime = (rand()%2)*1000000;
        spawnEvent.mCount = 0;
        spawnEvent.mSpawnId = rand() % getSpawnPointCount();
        // Pick a random amount of points to assign to a group of these enemies.
        uint32_t groupPoints = rand() % pointsRemain;
        pointsRemain-= groupPoints;
        while(groupPoints >= enemyTemplateCount.mEnemyTemplate->mPointValue)
        {
            // Add another enemy to this spawn event.
            ++spawnEvent.mCount;

            // Reset the noOp pass counter.
            noOpPassCount = 0;
    
            // Track that another of this enemy type has been added.
            ++enemyTemplateCount.mCount;

            // Remove points from this group.
            groupPoints-= enemyTemplateCount.mEnemyTemplate->mPointValue;
            
            // Track how many total points have been scheduled for this wave.
            pointsAdded+= enemyTemplateCount.mEnemyTemplate->mPointValue;

            // If the maximum number of enemies of this type has been reached for this wave.
            if(enemyTemplateCount.mCount >= enemyTemplateCount.mEnemyTemplate->mMaxWaveCount)
            {
                // Remove this enemyTemplate from the possible canidates.
                dprintf("Reached instance cap");
                for(EnemyTemplateCountVector::iterator it = templates.begin(); it!=templates.end(); ++it)
                {
                    if((*it).mEnemyTemplate == enemyTemplateCount.mEnemyTemplate)
                    {
                        dprintf("  removed");
                        templates.erase(it);

                        break;
                    }
                }
                
                groupPoints = 0;
            }
        }

        if(spawnEvent.mCount)
        {
            // At least 1 a second.
            spawnEvent.mDuration = 1+(rand()%spawnEvent.mCount)*1000000;
            addSpawnEvent(spawnEvent);
        }
    }

    dprintf("targetPoints: %u addedPoints: %u pointsRemain: %u\tnoOpPassCount: %d\n", pointValue, pointsAdded, pointsRemain, noOpPassCount);
    return pointsAdded;
}


} // namespace pammo