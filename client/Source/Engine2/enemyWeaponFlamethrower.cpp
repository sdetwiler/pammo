#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponFlamethrower.h"

namespace pammo
{

void enemyWeaponFlamethrowerFireParticleCb(Particle* p, ParticleSystem* system)
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

void enemyWeaponFlamethrowerFireCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

	doDamage(self, other, Fire, 10.0f);
}

void enemyWeaponFlamethrowerCb(Enemy* e, EnemyWeapon* w, EnemyManager* manager)
{
    FlamethrowerWeaponData* data = (FlamethrowerWeaponData*)w->mData;

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
    if(!p) return;

    // Set flamethrower specific particle properties.
    p->mCallback = enemyWeaponFlamethrowerFireParticleCb;
    p->mAlpha = 1.0f;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.makeDirty();
        
    // Properties about fire particle bodies.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponFlamethrowerFireCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 1;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(p->mImage.mRotation);
}

} // namespace pammo