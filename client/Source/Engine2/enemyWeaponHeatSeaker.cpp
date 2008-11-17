#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponHeatSeaker.h"

namespace pammo
{

struct HeatSeakerParticleData
{
    float mMaxRotation;
    HeatSeakerWeaponData* mWeaponData;
};

void enemyWeaponHeatSeakerParticleTailCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mSize *= 1.03f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.10f;
}

void enemyWeaponHeatSeakerMissleParticleCb(Particle* p, ParticleSystem* system)
{
    float mag = magnitude(p->mBody->mCenter - p->mStartPosition);
    if(mag>p->mMaxDistance)
    {
        gWorld->getParticleSystem()->initSmokeParticle(p->mBody->mCenter, 0, Vector2());
        system->removeParticle(p);
        return;
    }

    HeatSeakerParticleData* data = (HeatSeakerParticleData*)p->mData;
    Vector2 heading = gWorld->getPlayer()->getCenter() - p->mBody->mCenter;
    float rot = atan2(heading.y, heading.x);
    float deltaRot = rot - p->mImage.mRotation;
    if(deltaRot > data->mMaxRotation)
        deltaRot = 0;
    else if(deltaRot < -data->mMaxRotation)
        deltaRot = 0;

    p->mImage.mRotation += deltaRot;

    p->mBody->mVelocity = (Vector2(1.0f,0.0f) * Transform2::createRotation(p->mImage.mRotation)) * magnitude(p->mVelocity);
    p->mImage.mCenter = p->mBody->mCenter; 
    p->mImage.makeDirty();

    Particle* tailP = gWorld->getParticleSystem()->addParticle(2, false);
    if(!tailP)
        return;

    // Setup image properties.
    tailP->mImage.mCenter = p->mBody->mCenter;
    tailP->mImage.mRotation = p->mImage.mRotation+(M_PI/2.0f);
    tailP->mImage.setImage(gImageLibrary->getImage(PARTICLE_HEATSEAKER_TAIL_00));
    tailP->mImage.makeDirty();

    // Set jet flame specific particle properties.
    tailP->mCallback = enemyWeaponHeatSeakerParticleTailCb;
    tailP->mAlpha = 1.0f;
}

void enemyWeaponHeatSeakerShapeCollisionCb(Body* self, Shape* other, Contact* contact, bool* response)
{
    response = false;
    Particle* p = (Particle*)self->mUserArg;
    gWorld->getParticleSystem()->removeParticle(p);
    gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
}

void enemyWeaponHeatSeakerCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = false;

    Particle* p = (Particle*)self->mUserArg;
    HeatSeakerParticleData* data = (HeatSeakerParticleData*)p->mData;
    doDamage(self, other, HeatSeakerMissle, data->mWeaponData->mDamage);

    gWorld->getParticleSystem()->removeParticle(p);
    gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
}

void enemyWeaponHeatSeakerCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    HeatSeakerWeaponData* data = (HeatSeakerWeaponData*)w->mData;

    enemyWeaponTurretUpdate(e, w, manager, &data->mTurret);

    uint64_t now = getTime();
    if(data->mFireStateChange < now)
    {
        data->mFiring = !data->mFiring;
        float period = 2.0f * 1000000.0f;// * (((float)(rand()%100))/50.0f);
        float duration;
        duration = period * data->mFireRate;
        if(!data->mFiring)
        {
            duration = period - duration;
        }

        data->mFireStateChange = now + (uint64_t)duration;
    }
    if(data->mFiring == false)
    {
        return;
    }

    data->mFiring = false;

    Particle* p = NULL;
    enemyWeaponTurretGetParticleWithBody(e, w, manager, &data->mTurret, &p);
    if(!p)
        return;
    HeatSeakerParticleData* particleData = (HeatSeakerParticleData*)&(p->mData);
    particleData->mMaxRotation = (M_PI*data->mAccuracy);
    particleData->mWeaponData = data;

    // Set HeatSeaker specific particle properties.
    p->mCallback = enemyWeaponHeatSeakerMissleParticleCb;
    p->mAlpha = 1.0f;
    p->mVelocity = Vector2(300.0f, 0);    

    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_HEATSEAKER_00));

    Vector2 heading = gWorld->getPlayer()->getCenter() - p->mBody->mCenter;
    float rot = atan2(heading.y, heading.x);
    p->mImage.mRotation = rot;
    p->mImage.makeDirty();
        
    // Properties about heat seaker particle bodies.
    p->mBody->mUserArg = p;
    p->mBody->mProperties = kEnemyBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerBulletCollisionProperties | kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponHeatSeakerCollisionCb;
    p->mBody->mShapeCallback = enemyWeaponHeatSeakerShapeCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 5;
    p->mBody->mMass = 10.0f;
    p->mBody->mVelocity = e->mBody->mVelocity + p->mVelocity * Transform2::createRotation(p->mImage.mRotation);
    p->mStartPosition = p->mBody->mCenter;
    p->mMaxDistance = data->mMaxDistance;
    
    AudioInstance* instance = gAudioLibrary->getAudioInstance(AUDIO_ROCKETLAUNCHER);
    if(instance)
    {
        gAudioLibrary->playAudioInstance(instance, PLAY_ONCE, true);
        alSource3f(instance->mSource, AL_POSITION, e->mBody->mCenter.x, e->mBody->mCenter.y, 0.0f);    
    }
    
}

} // namespace pammo