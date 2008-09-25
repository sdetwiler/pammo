#include "gooWeapon.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "particleSystem.h"
#include "imageLibrary.h"
#include "enemyManager.h"

namespace pammo
{

// Forward declair collision and particle callbacks.
void gooBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response);
void gooBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response);
void gooBulletParticleCallback(Particle* p, ParticleSystem* system);
void gooSprayParticleCallback(Particle* p, ParticleSystem* system);

GooWeapon::GooWeapon()
    : Weapon()
{
    mIcon.setImage(gImageLibrary->reference("data/interface/gooWeaponIcon.png"));
}

GooWeapon::~GooWeapon()
{
}

void GooWeapon::select()
{
}

void GooWeapon::deselect()
{
}

struct GooParticleData
{
    int mDirection;
    uint64_t mExpireTime;
};

void GooWeapon::fire()
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(0);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = gooBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    GooParticleData* particleData = (GooParticleData*)p->mData;
    if(rand()%2)
        particleData->mDirection = -1;
    else
        particleData->mDirection = 1;

    particleData->mExpireTime = getTime() + 2000000;

    // Choose some numbers.
    float f = 10.0f;
    float r = 1.0f/f - ((rand()%100)/(f*90)) ;
    float initialRotation = player->mTurret.mRotation - M_PI/2.0f;
    
    // Setup image.
	int i=rand()%3;
	char filename[256];
	float velocity = 0;//130;
	sprintf(filename, "data/particles/goo/0%d.png", i);
	velocity+=((rand()%10)/10.0f);
	
	p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.mCenter = player->mTurretTip + Vector2(40, 0) * Transform2::createRotation(initialRotation+r);
    p->mImage.mRotation = initialRotation+r;
    p->mImage.makeDirty();
        
    // Properties about goo bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = gooBulletCollisionCallback;
    p->mBody->mShapeCallback = gooBulletShapeCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 16;
    p->mBody->mMass = 10;
    p->mBody->mCenter = p->mImage.mCenter;
    Vector2 v = Vector2(velocity, 0) * Transform2::createRotation(p->mImage.mRotation);
    p->mBody->mVelocity = v;


    // Nozzle spray.
    p = gWorld->getParticleSystem()->addParticle(2);
    if(!p) 
        return;
	p->mImage.setImage(gImageLibrary->reference(filename));
    p->mImage.mSize *= 0.5f;
    p->mImage.mCenter = player->mTurretTip + Vector2(4, 0) * Transform2::createRotation(initialRotation+r);
    p->mImage.mRotation = initialRotation + r;
    p->mAlpha = 1.0f;
    p->mImage.makeDirty();
    p->mVelocity =  Vector2(velocity+10.0f, 0) * Transform2::createRotation(initialRotation+r);
    p->mCallback = gooSprayParticleCallback;

}

void gooBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceMe = false;
	response->mBounceThem = false;
	
	doDamage(self, other, Goo, 1.0f);
}
	
void gooBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    *response = true;
}

void gooSprayParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mSize *= 1.2f;
    if(p->mImage.mSize.x >= 32.0f)
        system->removeParticle(p);

    p->mImage.mCenter+= p->mVelocity;
    p->mImage.makeDirty();

}

void gooBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    GooParticleData* particleData = (GooParticleData*)p->mData;

    
    float s;
    uint64_t now = getTime();
    p->mAlpha-=0.01f;
    if(now >= particleData->mExpireTime)
    {
        p->mAlpha-=0.02f;
        s = 0.95;     
        if(p->mAlpha <= 0.2)
	    {
            // Once it gets down to 0.2 transparency, turn off collision.
		    p->mBody->mProperties = 0;
		    p->mBody->mCollideProperties = 0;
	    }
	    if(p->mAlpha <= 0)
        {
		    system->removeParticle(p);
            return;
        }
    }
    else
    {
        s = (1.005 + ((float)(rand()%100)/10000.0f));
    }

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += (particleData->mDirection*0.015f);
    p->mImage.mSize *= s;
    p->mBody->mRadius *= s;

    p->mImage.makeDirty();

}
	

}

