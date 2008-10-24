#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponTrebuchet.h"

namespace pammo
{
struct TrebuchetBallParticleData
{
    uint64_t mStartTime;
    TrebuchetWeaponData* mWeaponData;
    Particle* mShadow;
};
void enemyWeaponTrebuchetBallShadowParticleCb(Particle* p, ParticleSystem* system)
{

}

void enemyWeaponTrebuchetBallParticleCb(Particle* p, ParticleSystem* system)
{
    TrebuchetBallParticleData* particleData = (TrebuchetBallParticleData*)p->mData;

    uint64_t now = getTime();
    float Di = 1.0f;
    float Vi = 6.0f;
    float G = 9.81f;
    float dt = ((float)(now - particleData->mStartTime))/1000000.0f;
    float distance = Di + ((Vi*dt) - (.5*G*(dt*dt)));

    if(distance <= Di*2.0f)
    {
        p->mBody->mCollideProperties = kPlayerCollisionProperties;
    }

    if(distance <= Di)
    {
        if(particleData->mShadow)
            gWorld->getParticleSystem()->removeParticle(particleData->mShadow);
            
        gWorld->getParticleSystem()->initExplosionParticle(p->mImage.mCenter);
		system->removeParticle(p);
		return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mSize = 8.0f * distance;
    p->mImage.makeDirty();

    if(particleData->mShadow)
    {
        particleData->mShadow->mImage.mRotation = p->mImage.mRotation;
        particleData->mShadow->mImage.mCenter = p->mImage.mCenter - (Vector2(4.0f, -7.0f)*distance);
        particleData->mShadow->mImage.mSize = 16.0f * distance;
        particleData->mShadow->mImage.makeDirty();
    }
}

void enemyWeaponTrebuchetBallCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    //dprintf("trebuchetBall collision");
    Particle* p = (Particle*)self->mUserArg;
	response->mBounceThem = true;
	response->mBounceMe = true;

    TrebuchetBallParticleData* particleData = (TrebuchetBallParticleData*)p->mData;

	doDamage(self, other, Grenade, 10.0f);

	gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
    if(particleData->mShadow)
        gWorld->getParticleSystem()->removeParticle(particleData->mShadow);
    gWorld->getParticleSystem()->removeParticle(p);
}

void enemyWeaponTrebuchetCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    TrebuchetWeaponData* data = (TrebuchetWeaponData*)w->mData;

    enemyWeaponTurretUpdate(e, w, manager, &data->mTurret);

    uint64_t now = getTime();

    uint64_t delta = now-data->mLastFire;
    if((float)delta/1000000.0f < data->mFireRate)
    {
        return;
    }
    data->mLastFire = now;

    Particle* p = NULL;
    enemyWeaponTurretGetParticleWithBody(e, w, manager, &data->mTurret, &p);
    if(!p)
        return;

    // Set basic particle properties.
    p->mCallback = enemyWeaponTrebuchetBallParticleCb;
    p->mAlpha = 1.0f;
    
    p->mMaxDistance = data->mMaxDistance;

    // Properties about trebuchet ball particles.
    TrebuchetBallParticleData* particleData = (TrebuchetBallParticleData*)p->mData;
    particleData->mStartTime = now;

    // Properties about ball particles.
    p->mBody->mProperties = kEnemyBulletCollisionProperties;
    p->mBody->mCollideProperties = 0;//kPlayerCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponTrebuchetBallCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 8;
    p->mBody->mMass = 20;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(180, 0) * Transform2::createRotation(e->mController.mRotation);
    p->mBody->mUserArg = p;

    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_BALL_00));
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();

    particleData->mShadow = gWorld->getParticleSystem()->addParticle(0, false);
    if(!particleData->mShadow) return;
    
    particleData->mShadow->mAlpha = 0.5f;
    particleData->mShadow->mCallback = enemyWeaponTrebuchetBallShadowParticleCb;
    particleData->mShadow->mImage.setImage(gImageLibrary->getImage(PARTICLE_SHADOW_00));
    particleData->mShadow->mImage.mCenter = p->mImage.mCenter;
    particleData->mShadow->mImage.mRotation= p->mImage.mRotation;
    particleData->mShadow->mImage.makeDirty();
}

} // namespace pammo