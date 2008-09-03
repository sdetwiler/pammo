#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyWeaponHeatSeaker.h"

namespace pammo
{

void enemyWeaponHeatSeakerParticleTailCb(Particle* p, ParticleSystem* system)
{
    //float r = ((rand()%100)/200.0f);
  //  p->mImage.mRotation += r;
    p->mImage.mSize *= 1.03f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.10f;
}
void enemyWeaponHeatSeakerFireParticleCb(Particle* p, ParticleSystem* system)
{
    
    float mag = magnitude(p->mBody->mCenter - p->mStartPosition);
    if(mag>p->mMaxDistance)
    {
        system->removeParticle(p);
        return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.makeDirty();

    Particle* tailP = gWorld->getParticleSystem()->addParticle(2);//WithBody(2);
    if(!tailP)
        return;

    // Calculate center. Vehicle center, plus particle offset rotated for vehicle, plus turret rotated for direction.
    Vector2 center = p->mBody->mCenter
                     + 0.0f
                     * Transform2::createRotation(p->mImage.mRotation);

    // Setup image properties.
    tailP->mImage.mCenter = center;
    tailP->mImage.mRotation = p->mImage.mRotation+(M_PI/2.0f);
    tailP->mImage.setImage(gImageLibrary->reference("data/particles/heatSeaker/tail/00.png"));
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

	doDamage(self, other, Fire, 10.0f);

    Particle* p = (Particle*)self->mUserArg;
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

        data->mFireStateChange = now + duration;
    }
    if(data->mFiring == false)
    {
        return;
    }

    data->mFiring = false;

    Particle* p = NULL;
    enemyWeaponTurretGetParticleWithBody(e, w, manager, &data->mTurret, &p);

    // Set HeatSeaker specific particle properties.
    p->mCallback = enemyWeaponHeatSeakerFireParticleCb;
    p->mAlpha = 1.0f;
    
    // Setup image.
    char filename[256];
    sprintf(filename, "data/particles/heatSeaker/00.png");
    p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.makeDirty();
        
    // Properties about heat seaker particle bodies.
    p->mBody->mUserArg = p;
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mBodyCallback = enemyWeaponHeatSeakerCollisionCb;
    p->mBody->mShapeCallback = enemyWeaponHeatSeakerShapeCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 5;
    p->mBody->mMass = 10.0f;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(400, 0) * Transform2::createRotation(p->mImage.mRotation-.2f);
    p->mStartPosition = p->mBody->mCenter;
    p->mMaxDistance = data->mMaxDistance;
}

} // namespace pammo