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
#include "enemyWeaponDamageTouch.h"
#include "enemyWeaponMachineGun.h"
#include "enemyWeaponHeatSeaker.h"
#include "enemyWeaponMineLayer.h"

#include "enemyParticleJetFlame.h"
#include "enemyParticleSmoke.h"

#include "camera.h"
#include "imageLibrary.h"
#include "audioLibrary.h"
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
    *p = gWorld->getParticleSystem()->addParticleWithBody(2, false);
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
    (*p)->mStartPosition = data->mTurretTip;
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
   
    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
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
{
    DriveByBehaviorData* data = (DriveByBehaviorData*)e->mBehavior.mData;

    // See how far vehicle has come.
    float distanceFromPlayer = magnitude(e->mBody->mCenter - gWorld->getPlayer()->getCenter());
    if(distanceFromPlayer > data->mDistance)
    {
        float distanceFromTurn = magnitude(e->mBody->mCenter - data->mTurnPoint);
        if(distanceFromTurn < data->mDistance/2.0f)
            return;

        float r2 = gWorld->getPlayer()->mBody->mRadius * 3.0f;
        if(rand()%2)
            r2*=-1.0f;

        Vector2 heading = (gWorld->getPlayer()->getCenter() + Vector2(r2, r2)) - e->mBody->mCenter;
        data->mTurnPoint = e->mBody->mCenter;
        float rot = atan2(heading.y, heading.x);
        e->mController.mRotationTarget = rot;
        e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
    }
    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

}

void behaviorCampCb(Enemy* e, EnemyManager* manager)
{}

void behaviorKamikazeCb(Enemy* e, EnemyManager* manager)
{
    KamikazeBehaviorData* data = (KamikazeBehaviorData*)e->mBehavior.mData;

    // Every 60 frames, adjust the bias.
    if(!data->mBiasUpdate)
    {
        data->mBias = (((float)(rand()%100))- 50.0f)/100.0f;
        data->mBiasUpdate = 30 * 2;
    }
    --data->mBiasUpdate;
    e->mController.update();

    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float rot = atan2(heading.y, heading.x);
    rot += data->mBias;
    if(rot < 0) 
        rot += (float)M_PI*2;
    e->mController.mRotationTarget = rot;
    e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
}

void behaviorSpinningTopCb(Enemy* e, EnemyManager* manager)
{
    SpinningTopBehaviorData* data = (SpinningTopBehaviorData*)e->mBehavior.mData;

    data->mRotation+=.7f;
    if(data->mRotation > M_PI*2.0f)
        data->mRotation-= M_PI*2.0f;

    e->mController.update();

    e->mEntity.mRotation = data->mRotation + e->mController.mRotation + ((float)M_PI/2.0f);
    if(e->mEntity.mRotation > M_PI*2.0f)
        e->mEntity.mRotation -= M_PI*2.0f;

    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float rot = atan2(heading.y, heading.x);
    e->mController.mRotationTarget = rot;
    e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;
}

void behaviorPounceAndStalkCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
//    dprintf("Collision");
    Enemy* e = (Enemy*)self->mUserArg;
    PounceAndStalkBehaviorData* data = (PounceAndStalkBehaviorData*)e->mBehavior.mData;
    data->mDistance = 0.0f;
    e->mController.mAcceleration = 0.0f;
    data->mInCollision = true;
    if(data->mState = PounceAndStalkBehaviorData::Move)
    {
        response->mBounceMe = true;
        response->mBounceThem = true;
    }
    else
    {
        response->mBounceMe = false;
        response->mBounceThem = false;
    }
}

void behaviorPounceAndStalkShapeCollisionCb(Body* self, Shape* other, Contact* contact, bool* response)
{
//    dprintf("Shape Collision");
    Enemy* e = (Enemy*)self->mUserArg;
    PounceAndStalkBehaviorData* data = (PounceAndStalkBehaviorData*)e->mBehavior.mData;
    data->mDistance = 0.0f;
    e->mController.mAcceleration = 0.0f;
    data->mInCollision = true;

    if(data->mState == PounceAndStalkBehaviorData::Move)
    {
        *response = true;
    }
    else
    {
        *response = false;
    }
}

void behaviorPounceAndStalk(Enemy* e, EnemyManager* manager)
{
    Vector2 direction;
    float d;
    PounceAndStalkBehaviorData* data = (PounceAndStalkBehaviorData*)e->mBehavior.mData;

    // Assume enemy is on the ground.
    data->mShadow->mImage.mCenter = e->mEntity.mCenter - (Vector2(5.0f, -8.0f));
    data->mShadow->mImage.makeDirty();

    switch(data->mState)
    {
    // Turn to another direction.
    case PounceAndStalkBehaviorData::RotateToMove:
//        dprintf("RotateToMove");
        direction = gWorld->getPlayer()->mBody->mCenter - e->mBody->mCenter;
        e->mController.mAcceleration = 0.0f;
        e->mController.mRotationTarget = atan2(direction.y, direction.x);

        data->mState = PounceAndStalkBehaviorData::PreMoveRotate;
        break;
    // Turn.    
    case PounceAndStalkBehaviorData::PreMoveRotate:
//        dprintf("PreMoveRotate");
        if(fabs(e->mController.mRotation - e->mController.mRotationTarget)<0.001f)
        {
            data->mDistance = (float)(rand()%200);
            data->mOrigin = e->mBody->mCenter;

            e->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
            e->mBody->mProperties = kEnemyCollisionProperties;

            data->mChillUntil = getTime() + (rand()%4000)*1000;
            data->mState = PounceAndStalkBehaviorData::CheckCollision;
        }
        break;
    case PounceAndStalkBehaviorData::CheckCollision:
        if(getTime()<data->mChillUntil)
            break;

//        dprintf("CheckCollision");
        if(data->mInCollision)
        {
            e->mBody->mCollideProperties = 0;
            e->mBody->mProperties = 0;

            data->mState = PounceAndStalkBehaviorData::RotateToJump;
        }
        else
        {
            e->mController.mAcceleration = data->mSpeed * e->mBody->mMass;

            e->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
            e->mBody->mProperties = kEnemyCollisionProperties;

            data->mState = PounceAndStalkBehaviorData::Move;
        }
        break;
    // Head in a direction for a bit.
    case PounceAndStalkBehaviorData::Move:
//        dprintf("Move");
        e->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
        d = magnitude(data->mOrigin - e->mBody->mCenter);
        if(d >= data->mDistance)
        {
            e->mBody->mCollideProperties = 0;
            e->mBody->mProperties = 0;

            data->mState = PounceAndStalkBehaviorData::RotateToJump;
        }
        break;
    // Turn to another direction.
    case PounceAndStalkBehaviorData::RotateToJump:
//        dprintf("RotateToJump");
        e->mController.mAcceleration = 0.0f;
        direction = gWorld->getPlayer()->mBody->mCenter - e->mBody->mCenter;
        e->mController.mRotationTarget = atan2(direction.y, direction.x);

        data->mState = PounceAndStalkBehaviorData::PreJumpRotate;
        break;
    // Rotate.
    case PounceAndStalkBehaviorData::PreJumpRotate:
//        dprintf("PreJumpRotate");
        if(fabs(e->mController.mRotation - e->mController.mRotationTarget)<0.001f)
        {
            data->mJumpStart = getTime();
            data->mOrigin = e->mBody->mCenter;
            data->mState = PounceAndStalkBehaviorData::Jump;
        }
        break;
    // Jump.
    case PounceAndStalkBehaviorData::Jump:

        e->mController.mAcceleration = data->mSpeed * e->mBody->mMass * 2.2f;

        uint64_t now = getTime();
        float Di = 1.0f;
        float Vi = 3.5f;
        float G = 9.81;
        float dt = ((float)(now - data->mJumpStart))/1000000.0f;
        float distance = Di + ((Vi*dt) - (.5*G*(dt*dt)));

        if(distance <= Di*.6667f)
        {
            e->mEntity.mSize = e->mImages[0]->mSize;
            data->mState = PounceAndStalkBehaviorData::RotateToMove;
		    return;
        }

        e->mEntity.mSize = 32.0f * distance;


        data->mShadow->mImage.mRotation = e->mEntity.mRotation;
        data->mShadow->mImage.mCenter = e->mEntity.mCenter - (Vector2(5.0f, -8.0f)*distance);
        data->mShadow->mImage.mSize = 32.0f * distance;
        data->mShadow->mImage.makeDirty();

        break;
    };

    data->mInCollision = false;

    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();
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
    e->mEntity.setImageWithoutSize(e->mImages[e->mCurrImage]);
    e->mShadow.mAlpha = 0.5f;
    e->mShadow.mSize = e->mImages[0]->mSize;
    e->mShadow.mRotation = e->mController.mRotation + (M_PI/2.0f);
    e->mShadow.mCenter = e->mEntity.mCenter + Vector2(-4.0f, 4.0f);
    e->mShadow.makeDirty();
   
    
    // e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();
}

void enemyDamageCb(Enemy* e, ParticleType type, float amount)
{
	e->mHealth-=amount;

	if(e->mHealth <=0)
	{
        AudioInstance* instance = gAudioLibrary->getAudioInstance(AUDIO_EXPLOSION);
        if(instance)
            gAudioLibrary->playAudioInstance(instance, true);

        gWorld->getParticleSystem()->initExplosionParticle(e->mBody->mCenter+Vector2(5,5));
		gWorld->getParticleSystem()->initExplosionParticle(e->mBody->mCenter);
		gWorld->getParticleSystem()->initRubbleParticle(e->mBody->mCenter);
        gWorld->getPlayer()->mScore += e->mPointValue;
        
        gWorld->getEnemyManager()->removeEnemy(e);
		e->mDamageCb = NULL;
        e->mUpdateCb = NULL;
	}
}

void pounceAndStalkShadowParticleCallback(Particle* p, ParticleSystem* system)
{
}

void enemyPounceAndStalkDamageCb(Enemy* e, ParticleType type, float amount)
{
    PounceAndStalkBehaviorData* data = (PounceAndStalkBehaviorData*)&e->mBehavior.mData;

    enemyDamageCb(e, type, amount);

	if(e->mHealth <=0)
	{
        gWorld->getParticleSystem()->removeParticle(data->mShadow);
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
//    dprintf("addSpawnEvent");
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
        enemyTemplate->mImages[0] = gImageLibrary->getImage(atol(enemyTemplate->mImagePath));
        //        enemyTemplate->mImages[0] = gImageLibrary->getImage(enemyTemplate->mImagePath);
        enemyTemplate->mImageCount = 1;
    }    
    else
    {
        dprintf("TODO: Restore enemy template flipbook support. Path will be startIndex:count");
        enemyTemplate->mImageCount = 1;
        loadFlipbook(atol(enemyTemplate->mImagePath), enemyTemplate->mImageCount, enemyTemplate->mImages);
        //        loadFlipbook(enemyTemplate->mImagePath, enemyTemplate->mImages, ENEMY_MAX_IMAGE_COUNT, &enemyTemplate->mImageCount);
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
    }
    
    e->mShadow.setImage(gImageLibrary->getImage(PARTICLE_SHADOW_00));

    // SCD A dirty hack so I don't have to set size on the flipbook rotations to preserve scaling animations.
    e->mEntity.mSize = e->mImages[0]->mSize;

    e->mUpdateCb = enemyUpdateCb;
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
    case SpinningTop:
        e->mBehavior.mCb = behaviorSpinningTopCb;
        break;
    case PounceAndStalk:
        e->mBehavior.mCb = behaviorPounceAndStalk;
        e->mDamageCb = enemyPounceAndStalkDamageCb;
        e->mBody->mBodyCallback = behaviorPounceAndStalkCollisionCb;
        e->mBody->mShapeCallback = behaviorPounceAndStalkShapeCollisionCb;     

        ((PounceAndStalkBehaviorData*)&e->mBehavior.mData)->mShadow = gWorld->getParticleSystem()->addParticle(0, true);
        ((PounceAndStalkBehaviorData*)&e->mBehavior.mData)->mShadow->mAlpha = 0.5f;
        ((PounceAndStalkBehaviorData*)&e->mBehavior.mData)->mShadow->mCallback = pounceAndStalkShadowParticleCallback;
        ((PounceAndStalkBehaviorData*)&e->mBehavior.mData)->mShadow->mImage.setImage(gImageLibrary->getImage(PARTICLE_SHADOW_00));

        break;
    }
    
    // Weapons.
    e->mWeaponCount = enemyTemplate->mWeaponCount;
    for(uint32_t i=0; i<e->mWeaponCount; ++i)
    {
        memcpy(&e->mWeapon[i], &enemyTemplate->mWeapons[i].mWeapon, sizeof(EnemyWeapon));
        e->mWeapon[i].mEntity.setImageAndInit(gImageLibrary->getImage(atol(enemyTemplate->mWeapons[i].mImagePath)));
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
        case DamageTouch:
            e->mWeapon[i].mCb = enemyWeaponDamageTouchCb;
            e->mBody->mBodyCallback = enemyWeaponDamageTouchCollisionCb;
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

void EnemyManager::reset()
{
    Enemy* e;
    Enemy* next;
    e = mAddEnemies;
    while(e)
    {
        next = e->mNext;

		// Push onto avail stack.
		e->mPrev = NULL;
		e->mNext = mFreed;
		mFreed = e;

        e = next;
    }
    mAddEnemies = NULL;

    e = mEnemies;
    while(e)
    {
        next = e->mNext;
        removeEnemy(e);
        e = next;
    }
    mEnemies = NULL;

    mSpawnEvents.clear();
    mNextWaveScore = 0;
}

void EnemyManager::update()
{
    //mDrawHead = mDrawTail = 0;
    //return;
    
	while(mAddEnemies)
	{
		// Pop from add stack.
		Enemy* e = mAddEnemies;
		mAddEnemies = e->mNext;

        // Push onto active stack.
        e->mNext = mEnemies;
        if(e->mNext)
			e->mNext->mPrev = e;
        e->mPrev = NULL;
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
        wavePoints = createWave(wavePoints);
        wavePoints -= (wavePoints/4);
        mNextWaveScore+= wavePoints;
    }
    
    // Calculate the span of the screen.
    Camera* camera = gWorld->getCamera();
    float cameraLeft = camera->mCenter.x - camera->mSize.x/2;
    float cameraTop = camera->mCenter.y - camera->mSize.y/2;
    float cameraRight = camera->mCenter.x + camera->mSize.x/2;
    float cameraBottom = camera->mCenter.y + camera->mSize.y/2;
    
    // Reset draw iterators.
    mDrawHead = mDrawTail = 0;
    
    // Update enemies.
	Enemy* e = mEnemies;
    Minimap* minimap = gWorld->getMinimap();
    while(e)
	{
        // Call update.
	    e->mUpdateCb(e, this);
        
        // Mark on minimap.
        minimap->mark(e->mBody->mCenter, kMinimapEnemyMarker);
        
        // If the enemy is on the screen, add it to the draw list.
        float sx = e->mEntity.mSize.x/2;
        float sy = e->mEntity.mSize.y/2;
        float cx = e->mEntity.mCenter.x;
        float cy = e->mEntity.mCenter.y;
        
        // If it is inside the camera, add it to the draw list.
        if(    cx + sx > cameraLeft  && cy + sy > cameraTop
            && cx - sx < cameraRight && cy - sy < cameraBottom)
        {
            e->mDrawNext = 0;
            
            if(!mDrawHead)
            {
                mDrawHead = mDrawTail = e;
            }
            else
            {
                mDrawTail->mDrawNext = e;
                mDrawTail = e;
            }
        }
        
		e = e->mNext;
	}
}

void EnemyManager::draw()
{
    gWorld->getCamera()->set();
    
    // Draw shadows.
	Enemy* e = mDrawHead;
	while(e)
	{
        e->mShadow.draw();
        e = e->mDrawNext;
	}

    // Draw enemies.
    e = mDrawHead;
	while(e)
	{
        //e->mShadow.draw();
        e->mEntity.draw();
        for(uint32_t i=0; i<e->mWeaponCount; ++i)
            e->mWeapon[i].mEntity.draw();
        e = e->mDrawNext;
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
    if(mAddEnemies)
        mAddEnemies->mPrev = e;

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

typedef vector< EnemyTemplateCount > EnemyTemplateCountVector;

uint32_t EnemyManager::createWave(uint32_t pointValue)
{
    uint32_t pointsAdded = 0;
    dprintf("createWave %u points", pointValue);

    // Find all enemy templates that should be unlocked.
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


    int noOpPassCount = 0;
    static const int maxNoOps = 3;

    // While there are points left to distribute for this wave and there are available templates.
    uint32_t pointsRemain = pointValue;
    while(pointsRemain && (noOpPassCount < maxNoOps) && templates.size())
    {
        ++noOpPassCount;

        // Pick a random unlocked enemy and create a spawn event sometime in the near future.
        EnemyTemplateCount& enemyTemplateCount = templates[rand()%templates.size()];
        SpawnEvent spawnEvent;
        strcpy(spawnEvent.mEnemyName, enemyTemplateCount.mEnemyTemplate->mName);
        spawnEvent.mStartTime = (rand()%2)*1000000;
        spawnEvent.mCount = 0;
        spawnEvent.mSpawnId = rand() % getSpawnPointCount();

        dprintf("  Selected %s worth %u each", spawnEvent.mEnemyName, enemyTemplateCount.mEnemyTemplate->mPointValue);

        // Pick a random amount of points to try to assign to a group of these enemies.
        uint32_t groupPoints = (rand() % pointsRemain) + 1;
        if(groupPoints < enemyTemplateCount.mEnemyTemplate->mPointValue)
            groupPoints = enemyTemplateCount.mEnemyTemplate->mPointValue;
//        pointsRemain-= groupPoints;

        uint32_t pointsDistributed = 0;
        while(groupPoints >= enemyTemplateCount.mEnemyTemplate->mPointValue)
        {
            dprintf("    Distributing %u points to current group", groupPoints);

            // Add another enemy to this spawn event.
            ++spawnEvent.mCount;

            // Reset the noOp pass counter.
            noOpPassCount = 0;
    
            // Track that another of this enemy type has been added.
            ++enemyTemplateCount.mCount;

            // Remove points from this group.
            groupPoints-= enemyTemplateCount.mEnemyTemplate->mPointValue;
            
            // Track how many total points have been scheduled for this wave.
            pointsDistributed+= enemyTemplateCount.mEnemyTemplate->mPointValue;

            // If the maximum number of enemies of this type has been reached for this wave.
            if(enemyTemplateCount.mCount >= enemyTemplateCount.mEnemyTemplate->mMaxWaveCount)
            {
                // Remove this enemyTemplate from the possible canidates.
                dprintf("      Reached instance cap for this enemy type");
                for(EnemyTemplateCountVector::iterator it = templates.begin(); it!=templates.end(); ++it)
                {
                    if((*it).mEnemyTemplate == enemyTemplateCount.mEnemyTemplate)
                    {
                        dprintf("      ...removed");
                        templates.erase(it);

                        break;
                    }
                }
                
                groupPoints = 0;
            }
        }
        if(pointsDistributed < pointsRemain)
            pointsRemain-= pointsDistributed;
        else
            pointsRemain = 0;

        pointsAdded+= pointsDistributed;

        // If enemies exist in this spawn event.
        if(spawnEvent.mCount)
        {
            dprintf("  addSpawnEvent for %u enemies", spawnEvent.mCount);
            // At least 1 a second.
            spawnEvent.mDuration = 1+(rand()%spawnEvent.mCount)*1000000;
            addSpawnEvent(spawnEvent);
        }
            dprintf("%u points remain to be distributed for this wave", pointsRemain);

    }

    dprintf("targetPoints: %u addedPoints: %u pointsRemain: %u\tnoOpPassCount: %d\n", pointValue, pointsAdded, pointsRemain, noOpPassCount);
    return pointsAdded;
}


} // namespace pammo
