#include "player.h"
#include "world.h"
#include "physics.h"
#include "imageLibrary.h"
#include "enemyManager.h"
#include "enemyParticleSmoke.h"

namespace pammo
{

void enemyParticleSmokeParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    //p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= .995f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.085f;
}

void enemyParticleSmokeCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = false;
	response->mBounceMe = true;
}


void enemyParticleSmokeCb(Enemy* e, EnemyParticle* ep, EnemyManager* manager)
{
    SmokeParticleData* data = (SmokeParticleData*)ep->mData;

    Particle* p = NULL;

    // Get a particle.
    p = gWorld->getParticleSystem()->addParticleWithBody(2);
    if(!p)
        return;

    // Calculate center. Vehicle center, plus particle offset rotated for vehicle, plus turret rotated for direction.
    Vector2 center = e->mBody->mCenter
                     + data->mPosition
                     * Transform2::createRotation(e->mController.mRotation);

    // Setup image properties.
    p->mImage.mCenter = center;
    p->mImage.mRotation = data->mRotation;
    p->mImage.setImage(gImageLibrary->reference(ep->mImagePath));
    p->mImage.makeDirty();

    // Base properties about particle bodies.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mCenter = center;
        
    // Set jet flame specific particle properties.
    p->mCallback = enemyParticleSmokeParticleCb;
    p->mAlpha = 1.0f;
    
    // Properties about fire particle bodies.
    p->mBody->mBodyCallback = enemyParticleSmokeCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 1;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(data->mSpeed, 0) * Transform2::createRotation(data->mRotation);
}

} // namespace pammo