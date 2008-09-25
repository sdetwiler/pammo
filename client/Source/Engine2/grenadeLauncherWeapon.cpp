#include "grenadeLauncherWeapon.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "particleSystem.h"
#include "imageLibrary.h"
#include "enemyManager.h"

namespace pammo
{

// Forward declare collision and particle callbacks.
void grenadeLauncherBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response);
void grenadeLauncherBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response);
void grenadeLauncherBulletParticleCallback(Particle* p, ParticleSystem* system);

GrenadeLauncherWeapon::GrenadeLauncherWeapon()
    : Weapon()
{
    mLastFire = 0;
    mFireFrequency = 800000;
    mIcon.setImage(gImageLibrary->reference("data/interface/grenadeLauncherWeaponIcon.png"));
}

GrenadeLauncherWeapon::~GrenadeLauncherWeapon()
{
}

void GrenadeLauncherWeapon::select()
{
}

void GrenadeLauncherWeapon::deselect()
{
}

struct GrenadeLauncherParticleData
{
    int mDirection;
    uint64_t mStartTime;
};

void GrenadeLauncherWeapon::fire()
{
    uint64_t now = getTime();
    if(mLastFire + mFireFrequency > now)
        return;

    mLastFire = now;

    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(0);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = grenadeLauncherBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    GrenadeLauncherParticleData* particleData = (GrenadeLauncherParticleData*)p->mData;
    if(rand()%2)
        particleData->mDirection = -1;
    else
        particleData->mDirection = 1;

    particleData->mStartTime = getTime();

    // Choose some numbers.
    float f = 10.0f;
    float r = 0.0f;//1.0f/f - ((rand()%100)/(f*90)) ;
    float initialRotation = player->mController->mRotation + player->mFireDirection;
    
    // Setup image.
//	int i=rand()%3;
	char filename[256];
	float velocity = 130;
	sprintf(filename, "data/particles/grenade/00.png");
	velocity+=((rand()%10)/10.0f);
	
	p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.mCenter = player->mBody->mCenter + Vector2(0, 0) * Transform2::createRotation(initialRotation+r);
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    // Properties about grenade bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;// | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = grenadeLauncherBulletCollisionCallback;
    p->mBody->mShapeCallback = NULL;//grenadeLauncherBulletShapeCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 16;
    p->mBody->mMass = 10;
    p->mBody->mCenter = p->mImage.mCenter;
    Vector2 v = Vector2(velocity, 0) * Transform2::createRotation(initialRotation+r);
    p->mBody->mVelocity = v;
}

void grenadeLauncherBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceMe = false;
	response->mBounceThem = false;
	
	doDamage(self, other, Grenade, 30.0f);
}
	
void grenadeLaucherBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    *response = true;
}

void grenadeLauncherBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    GrenadeLauncherParticleData* particleData = (GrenadeLauncherParticleData*)p->mData;
    
    uint64_t now = getTime();
    float Di = 1.0f;
    float Vi = 6.0f;
    float G = 9.81; 
    float dt = ((float)(now - particleData->mStartTime))/1000000.0f;
    float distance = Di + ((Vi*dt) - (.5*G*(dt*dt)));
   
    if(distance <= Di*2.0f)
    {
        p->mBody->mCollideProperties = kEnemyCollisionProperties;
    }

    if(distance <= Di)
    {
        gWorld->getParticleSystem()->initExplosionParticle(p->mImage.mCenter);
		system->removeParticle(p);
		return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mSize = 8.0f * distance;
    p->mImage.makeDirty();
}
	

}
