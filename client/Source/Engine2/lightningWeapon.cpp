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
void lightningGlowParticleCallback(Particle* p, ParticleSystem* system);

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
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = lightningBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    // Choose some numbers.
    float f = 10.0;
    float r = 1.0/f - ((rand()%100)/(f*90)) ;
    float initialRotation = atan2(player->mFireDirection.y, player->mFireDirection.x);
    
    // Setup image.
	int i=rand()%3;
	char filename[256];
	float velocity = 130;
	sprintf(filename, "data/particles/lightning0%d.png", i);
    if(i)
	{
		velocity+=100+(rand()%100);
	}
	
	p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.mCenter = player->mBody->mCenter + Vector2(10, 0) * Transform2::createRotation(initialRotation+r);
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    // Properties about lightning bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;
    p->mBody->mBodyCallback = lightningBulletCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 20;
    p->mBody->mMass = 15;
    p->mBody->mCenter = p->mImage.mCenter + Vector2(20, 0) * Transform2::createRotation(initialRotation+r);
    p->mBody->mVelocity = player->mBody->mVelocity + Vector2(velocity, 0) * Transform2::createRotation(initialRotation+r);

    // Get a particle.
	p = gWorld->getParticleSystem()->addParticle(2);
    if(!p)
		return;
    

    // Set basic particle properties.
    p->mCallback = lightningGlowParticleCallback;
    p->mAlpha = 0.6f;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/lightningGlow00.png"));
    p->mImage.mCenter = player->mBody->mCenter;
    p->mImage.mRotation = initialRotation + (r*4);
    p->mImage.makeDirty();
        
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
    
    p->mAlpha-=0.08f;
    if(p->mAlpha <= 0)
        system->removeParticle(p);
}

void lightningGlowParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = gWorld->getPlayer()->getCenter();
	p->mImage.mSize*=1.4;
	p->mImage.makeDirty();
    
    p->mAlpha-=0.2f;
    if(p->mAlpha <= 0)
        system->removeParticle(p);
}


}

