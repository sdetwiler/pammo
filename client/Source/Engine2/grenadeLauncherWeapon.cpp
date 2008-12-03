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
void grenadeLauncherBulletShadowParticleCallback(Particle* p, ParticleSystem* system);

GrenadeLauncherWeapon::GrenadeLauncherWeapon()
    : Weapon()
{
    mLastFire = 0;
    mFireFrequency = 250000;
    mIcon.setImage(gImageLibrary->getImage(INTERFACE_ICON_WEAPON_GRENADELAUNCHER));
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
    int       mDirection;
    uint64_t  mStartTime;
    float     mRotation;
    Particle* mShadow;
};

void GrenadeLauncherWeapon::fire()
{
    uint64_t now = getTime();
    if(mLastFire + mFireFrequency > now)
        return;

    mLastFire = now;

    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2, false);
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
    particleData->mRotation = 0;

    // Choose some numbers.
    float r = (float)rand()/RAND_MAX * M_PI/8 - M_PI/16;
    float initialRotation = player->mTurret.mRotation - M_PI/2.0f;
    
    float velocity = 110 + (rand()%45);
    
    // Setup image.
	
	p->mImage.setImage(gImageLibrary->getImage(PARTICLE_GRENADE_00));
    p->mImage.mCenter = player->mTurretTip;
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    particleData->mShadow = gWorld->getParticleSystem()->addParticle(0, false);
    particleData->mShadow->mAlpha = 0.5f;
    particleData->mShadow->mCallback = grenadeLauncherBulletShadowParticleCallback;
    particleData->mShadow->mImage.setImage(gImageLibrary->getImage(PARTICLE_SHADOW_00));
    particleData->mShadow->mImage.mCenter = p->mImage.mCenter;
    particleData->mShadow->mImage.mRotation= p->mImage.mRotation;
    particleData->mShadow->mImage.makeDirty();

    // Properties about grenade bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;// | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = grenadeLauncherBulletCollisionCallback;
    p->mBody->mShapeCallback = NULL;//grenadeLauncherBulletShapeCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 50;
    p->mBody->mMass = 10;
    p->mBody->mCenter = p->mImage.mCenter;
    p->mBody->mVelocity = player->mBody->mVelocity/2 + Vector2(velocity, 0) * Transform2::createRotation(initialRotation+r);
    p->mBody->mUserArg = p;

    gWorld->getParticleSystem()->initSmokeParticle(player->mTurretTip, initialRotation, Vector2(1.0f, 0));

}

void grenadeLauncherBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
 //   if(self->mCollideProperties == 0)
 //       return;

    response->mBounceMe = false;
	response->mBounceThem = true;
    
	doDamage(self, other, Grenade, 50.0f);

    //Particle* p = (Particle*)self->mUserArg;
    //GrenadeLauncherParticleData* particleData = (GrenadeLauncherParticleData*)p->mData;


    //gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
    //gWorld->getParticleSystem()->removeParticle(particleData->mShadow);
	
    //gWorld->getParticleSystem()->removeParticle(p);
	
}
	
void grenadeLaucherBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    *response = true;
}
void grenadeLauncherBulletShadowParticleCallback(Particle* p, ParticleSystem* system)
{
}

void grenadeLauncherBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    GrenadeLauncherParticleData* particleData = (GrenadeLauncherParticleData*)p->mData;
    
    particleData->mRotation+=.7f;
    if(particleData->mRotation > M_PI*2.0f)
        particleData->mRotation-= M_PI*2.0f;

    uint64_t now = getTime();
    float Di = 1.0f;
    float Vi = 5.0f;
    float G = 9.81f; 
    float dt = ((float)(now - particleData->mStartTime))/1000000.0f;
    float distance = Di + ((Vi*dt) - (.5*G*(dt*dt)));
   
    if(distance <= Di*1.8f && dt > 0.5)
    {
        //printf("+");
        p->mBody->mProperties = kPlayerBulletCollisionProperties;
        p->mBody->mCollideProperties = kEnemyCollisionProperties;
    }
    else
    {
        //printf("-");
        p->mBody->mProperties = 0;
        p->mBody->mCollideProperties = 0;
    }


    if(distance <= Di)
    {
        gWorld->getParticleSystem()->initExplosionParticle(p->mImage.mCenter);
        AudioInstance* instance = gAudioLibrary->getAudioInstance(AUDIO_EXPLOSION1);
        if(instance)
        {
            gAudioLibrary->playAudioInstance(instance, PLAY_ONCE, true);
            alSource3f(instance->mSource, AL_POSITION, p->mImage.mCenter.x, p->mImage.mCenter.y, 0.0f);    
        }
        
        system->removeParticle(particleData->mShadow);
		system->removeParticle(p);
		return;
    }

    p->mImage.mRotation = particleData->mRotation;
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mSize = 10.0f * distance;
    p->mImage.makeDirty();

    particleData->mShadow->mImage.mRotation = p->mImage.mRotation;
    particleData->mShadow->mImage.mCenter = p->mImage.mCenter - (Vector2(2.0f, -3.5f)*distance);
    particleData->mShadow->mImage.mSize = 12.0f * distance;
    particleData->mShadow->mImage.makeDirty();
}
	

}
