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
};

void enemyWeaponTrebuchetBallParticleCb(Particle* p, ParticleSystem* system)
{
    TrebuchetBallParticleData* data = (TrebuchetBallParticleData*)p->mData;

    uint64_t now = getTime();
    float Di = 8.0f;
    float Vi = 50.0f;
    float G = 19.81; // Fantasyland... or Jupiter.
    float dt = ((float)(now - data->mStartTime))/1000000.0f;
    float distance = Di + ((Vi*dt) - ((G*dt*dt)/1.0f));

    if(distance <= Di*.667f)
    {
        p->mBody->mCollideProperties = kPlayerCollisionProperties;
    }

    if(distance <= Di)
    {
        //gWorld->getParticleSystem()->initExplosionParticle(p->mImage.mCenter);
		system->removeParticle(p);
		return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mSize = distance;
    p->mImage.makeDirty();
}

void enemyWeaponTrebuchetBallCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    Particle* p = (Particle*)self->mUserArg;
	response->mBounceThem = true;
	response->mBounceMe = true;

	doDamage(self, other, Fire, 10.0f);

	gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
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
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = 0;//kPlayerCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponTrebuchetBallCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 20;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(50, 0) * Transform2::createRotation(e->mController.mRotation);
    p->mBody->mUserArg = p;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/ball.png"));
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();
}

} // namespace pammo