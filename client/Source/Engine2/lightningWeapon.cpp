#include "lightningWeapon.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "particleSystem.h"
#include "imageLibrary.h"
#include "enemyManager.h"

namespace pammo
{

// Forward declair collision and particle callbacks.
void lightningBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response);
void lightningBulletParticleCallback(Particle* p, ParticleSystem* system);

LightningWeapon::LightningWeapon()
    : Weapon()
{
    mIcon.setImage(gImageLibrary->reference("data/interface/lightningWeaponIcon.png"));
}

LightningWeapon::~LightningWeapon()
{
}

void LightningWeapon::select()
{
}

void LightningWeapon::deselect()
{
}

void LightningWeapon::fire()
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody();
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = lightningBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    // Choose some numbers.
    float f = 3.0;
    float r = 1.0/f - ((rand()%100)/(f*90)) ;
    float initialRotation = atan2(player->mFireDirection.y, player->mFireDirection.x);
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/lightning00.png"));
    p->mImage.mCenter = player->mBody->mCenter;
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    // Properties about fire particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;
    p->mBody->mBodyCallback = lightningBulletCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 20;
    p->mBody->mMass = 15;
    p->mBody->mCenter = player->mBody->mCenter;
    p->mBody->mVelocity = player->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(initialRotation+r);
}

void lightningBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    response->mBounceMe = true;

	//doDamage(self, other, Fire, 10.0f);
    
    Enemy* e = (Enemy*)other->mUserArg;
    if(e->mDamageCb)
        e->mDamageCb(e, Lightning, 20);
}

void lightningBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
  //  p->mImage.mRotation += 0.065f;
   // p->mImage.mSize *= 1.05;
    p->mImage.makeDirty();
    
    p->mAlpha-=0.06f;
    if(p->mAlpha <= 0)
        system->removeParticle(p);
}

}
