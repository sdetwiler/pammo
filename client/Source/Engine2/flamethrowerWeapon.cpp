#include "flamethrowerWeapon.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "particleSystem.h"
#include "imageLibrary.h"
#include "enemyManager.h"

namespace pammo
{

// Forward declair collision and particle callbacks.
void flamethrowerBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response);
void flamethrowerBulletParticleCallback(Particle* p, ParticleSystem* system);

FlamethrowerWeapon::FlamethrowerWeapon()
    : Weapon()
{
    mIcon.setImage(gImageLibrary->reference("data/interface/flamethrowerWeaponIcon.png"));
}

FlamethrowerWeapon::~FlamethrowerWeapon()
{}
        
void FlamethrowerWeapon::select()
{
}
void FlamethrowerWeapon::deselect()
{
}

void FlamethrowerWeapon::fire()
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody();
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = flamethrowerBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    // Choose some numbers.
    float f = 8.0;
    float r = 1.0/f - ((rand()%100)/(f*50)) ;
    float initialRotation = atan2(player->mFireDirection.y, player->mFireDirection.x);
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.mCenter = player->mBody->mCenter;
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    // Properties about fire particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;
    p->mBody->mBodyCallback = flamethrowerBulletCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 20;
    p->mBody->mMass = 50;
    p->mBody->mCenter = player->mBody->mCenter;
    p->mBody->mVelocity = player->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(initialRotation+r);
}

void flamethrowerBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    response->mBounceMe = true;

	//doDamage(self, other, Fire, 10.0f);
    
    Enemy* e = (Enemy*)other->mUserArg;
    if(e->mDamageCb)
        e->mDamageCb(e, Fire, 10);
}

void flamethrowerBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= 1.05;
    p->mImage.makeDirty();
    
    p->mAlpha-=0.08f;
    if(p->mAlpha <= 0)
        system->removeParticle(p);
}

}