#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponMineLayer.h"

namespace pammo
{
struct MineParticleData
{
    uint64_t mExpireTime;
    MineLayerWeaponData* mWeaponData;
};

void enemyWeaponMineLayerMineParticleCb(Particle* p, ParticleSystem* system)
{
    uint64_t now = getTime();
    MineParticleData* data = (MineParticleData*)p->mData;
    if(data->mExpireTime <= now)
    {
        gWorld->getParticleSystem()->initExplosionParticle(p->mBody->mCenter);
        gWorld->getParticleSystem()->removeParticle(p);
    }
}

void enemyWeaponMineLayerCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    Particle* p = (Particle*)self->mUserArg;
	response->mBounceThem = true;
	response->mBounceMe = false;

    MineParticleData* data = (MineParticleData*)p->mData;
	doDamage(self, other, Mine, data->mWeaponData->mDamage);

	gWorld->getParticleSystem()->initExplosionParticle(p->mImage.mCenter);
	gWorld->getParticleSystem()->initRubbleParticle(p->mImage.mCenter);
    gWorld->getParticleSystem()->removeParticle(p);
}

void enemyWeaponMineLayerCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    MineLayerWeaponData* data = (MineLayerWeaponData*)w->mData;

    uint64_t now = getTime();

    // If stopping or stopped, don't drop mines.
    if(magnitude(e->mBody->mVelocity) < 1.0f)
        return;

    uint64_t delta = now-data->mLastFire;
    if((float)delta/1000000.0f < data->mFireRate)
    {
        return;
    }
    data->mLastFire = now;

    Particle* p = NULL;
    // Get a particle.
    p = gWorld->getParticleSystem()->addParticleWithBody(0);
    if(!p)
        return;
    
    // Set basic particle properties.
    p->mCallback = enemyWeaponMineLayerMineParticleCb;
    p->mAlpha = 1.0f;
    
        
    // Properties about mine particles.
    MineParticleData* particleData = (MineParticleData*)p->mData;
    particleData->mExpireTime = now + data->mMaxLifetime;
    particleData->mWeaponData = data;

    p->mBody->mProperties = kEnemyBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponMineLayerCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 20;
    p->mBody->mVelocity = Vector2(0.0f, 0.0f);
    p->mBody->mUserArg = p;
    p->mBody->mCenter = e->mEntity.mCenter;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/mine.png"));
    p->mImage.mAlpha = 1.0f;
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();
}

} // namespace pammo