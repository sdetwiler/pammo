#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponMachineGun.h"

namespace pammo
{

void enemyWeaponMachineGunFireParticleCb(Particle* p, ParticleSystem* system)
{
    
    float mag = magnitude(p->mBody->mCenter - p->mStartPosition);
    if(mag>p->mMaxDistance)
    {
        system->removeParticle(p);
        return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();
}

void enemyWeaponMachineGunShapeCollisionCb(Body* self, Shape* other, Contact* contact, bool* response)
{
    response = false;
    Particle* p = (Particle*)self->mUserArg;
    gWorld->getParticleSystem()->removeParticle(p);
    gWorld->getParticleSystem()->initSmokeParticle(self->mCenter, 0, self->mVelocity);
}

void enemyWeaponMachineGunCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = false;
	response->mBounceMe = false;

	doDamage(self, other, Fire, 10.0f);

    Particle* p = (Particle*)self->mUserArg;
    gWorld->getParticleSystem()->removeParticle(p);
}

void enemyWeaponMachineGunCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    MachineGunWeaponData* data = (MachineGunWeaponData*)w->mData;

    enemyWeaponTurretUpdate(e, w, manager, &data->mTurret);

    uint64_t now = getTime();
    if(data->mFireStateChange < now)
    {
        data->mFiring = !data->mFiring;
        float period = 2.0f * 1000000.0f * (((float)(rand()%100))/50.0f);
        float duration;
        duration = period * data->mFireRate;
        if(!data->mFiring)
        {
            duration = period - duration;
        }

        data->mFireStateChange = now + duration;
    }
    if(data->mFiring == false)
    {
        return;
    }

    Particle* p = NULL;
    enemyWeaponTurretGetParticleWithBody(e, w, manager, &data->mTurret, &p);

    // Set MachineGun specific particle properties.
    p->mCallback = enemyWeaponMachineGunFireParticleCb;
    p->mAlpha = 1.0f;
    
    // Setup image.
    char filename[256];
    sprintf(filename, "data/particles/machineGun/%02d.png", rand()%3);
    p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.makeDirty();
        
    // Properties about machine gun particle bodies.
    p->mBody->mUserArg = p;
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponMachineGunCollisionCb;
    p->mBody->mShapeCallback = enemyWeaponMachineGunShapeCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 5;
    p->mBody->mMass = .001;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(p->mImage.mRotation);
    p->mStartPosition = p->mBody->mCenter;
    p->mMaxDistance = data->mMaxDistance;
}

} // namespace pammo