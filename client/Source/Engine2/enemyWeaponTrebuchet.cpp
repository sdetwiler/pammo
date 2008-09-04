#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponTrebuchet.h"

namespace pammo
{

void enemyWeaponTrebuchetBallParticleCb(Particle* p, ParticleSystem* system)
{
    if(magnitude(p->mEndPosition - p->mStartPosition) < magnitude(p->mBody->mCenter - p->mStartPosition))
    {
		system->removeParticle(p);
		return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    float distance = magnitude(p->mImage.mCenter - p->mStartPosition)/magnitude(p->mEndPosition - p->mStartPosition);
    float x = (distance-0.5f)*1.5f;
    float y = (-(x*x))+1;
    p->mImage.mSize = 16 * y;
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

    uint64_t now = getTime();

    uint64_t delta = now-data->mLastFire;
    if((float)delta/1000000.0f < data->mFireRate)
    {
        return;
    }
    data->mLastFire = now;

    Particle* p = NULL;
    enemyWeaponTurretGetParticleWithBody(e, w, manager, &data->mTurret, &p);

    // Set basic particle properties.
    p->mCallback = enemyWeaponTrebuchetBallParticleCb;
    p->mAlpha = 1.0f;
    
    p->mEndPosition = e->mBody->mCenter + Vector2(data->mMaxDistance, 0) * Transform2::createRotation(e->mController.mRotation);

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/ball.png"));
    p->mImage.makeDirty();
        
    // Properties about ball particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kEnemyBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponTrebuchetBallCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 20;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(e->mController.mRotation);

    p->mBody->mUserArg = p;

}

} // namespace pammo