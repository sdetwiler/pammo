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
void lightningBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response);
void lightningBulletParticleCallback(Particle* p, ParticleSystem* system);
void lightningBulletFadeParticleCallback(Particle* p, ParticleSystem* system);
void lightningGlowParticleCallback(Particle* p, ParticleSystem* system);

    
struct LightningWeaponParticleData
{
    LightningWeapon* mWeapon;
};
        
    
LightningWeapon::LightningWeapon()
    : Weapon()
{
    mParticleCount = 0;
    mAudio = gAudioLibrary->getAudioInstance(AUDIO_LIGHTNINGGUN);
    mIcon.setImage(gImageLibrary->getImage(INTERFACE_ICON_WEAPON_LIGHTNINGGUN));
}

LightningWeapon::~LightningWeapon()
{
    gAudioLibrary->stopAudioInstance(mAudio);
    gAudioLibrary->closeAudioInstance(mAudio);
    
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
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2, false);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = lightningBulletParticleCallback;
    p->mAlpha = 1.0f;
    
    // Choose some numbers.
    float f = 10.0;
    float r = 1.0/f - ((rand()%100)/(f*90)) ;
//    float initialRotation = player->mFireDirection;
    float initialRotation = player->mTurret.mRotation - M_PI/2.0f;
    
    // Setup image.
	int i=rand()%PARTICLE_LIGHTNING_COUNT;
	float velocity = 130;
    if(i)
	{
		velocity+=100+(rand()%100);
	}
	
	p->mImage.setImage(gImageLibrary->getImage(PARTICLE_LIGHTNING_00 + i));
    p->mImage.mCenter = player->mTurretTip;
    p->mImage.mRotation = initialRotation + r;
    p->mImage.makeDirty();
        
    // Properties about lightning bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = lightningBulletCollisionCallback;
    p->mBody->mShapeCallback = lightningBulletShapeCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 8;
    p->mBody->mMass = 10;
    p->mBody->mCenter = p->mImage.mCenter + Vector2(8, 0) * Transform2::createRotation(initialRotation+r);
    p->mBody->mVelocity = player->mBody->mVelocity + Vector2(velocity, 0) * Transform2::createRotation(initialRotation+r);

    // Get a particle.
	p = gWorld->getParticleSystem()->addParticle(2, false);
    if(!p) return;
    

    // Set basic particle properties.
    p->mCallback = lightningGlowParticleCallback;
    p->mAlpha = 0.6f;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_LIGHTNING_GLOW_00));
    p->mImage.mCenter = player->mTurretTip;
    p->mImage.mRotation = initialRotation + (r*4);
    p->mImage.makeDirty();
    
    LightningWeaponParticleData* data = (LightningWeaponParticleData*)(p->mData);
    data->mWeapon = this;
    
    ++mParticleCount;
    if(mParticleCount == 1)
    {
        gAudioLibrary->playAudioInstance(mAudio, PLAY_FOREVER, false);
    }        

    if(gAudioLibrary->getAudioEnabled())
    {
        alSource3f(mAudio->mSource, AL_POSITION, p->mImage.mCenter.x, p->mImage.mCenter.y, 0.0f);
    }
}

void lightningBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceMe = true;
	response->mBounceThem = true;

    gWorld->getParticleSystem()->initSmokeParticle(contact->mContactPoint, 0.0f, Vector2(0,0), 5);

    doDamage(self, other, Lightning , 20.0f);

    // Stop in place and fade to dark.
	Particle* me = (Particle*)self->mUserArg;
    gWorld->getPhysics()->removeBody(me->mBody);
	me->mBody = 0;
	me->mCallback = lightningBulletFadeParticleCallback;
}
	
void lightningBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    gWorld->getParticleSystem()->initSmokeParticle(contact->mContactPoint, 0.0f, Vector2(0,0), 5);

    // When particles hit a wall, stop in place and fade to dark.
	Particle* me = (Particle*)self->mUserArg;
    gWorld->getPhysics()->removeBody(me->mBody);
	me->mBody = 0;
	me->mCallback = lightningBulletFadeParticleCallback;
}

void lightningBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    // p->mImage.mRotation += 0.065f;
    // p->mImage.mSize *= 1.05;
    p->mImage.makeDirty();
    
    p->mAlpha-=0.08f;
    if(p->mAlpha <= 0.2)
	{
        // Once it gets down to 0.2 transparency, turn off collision.
		p->mBody->mProperties = 0;
		p->mBody->mCollideProperties = 0;
	}
	if(p->mAlpha <= 0)
		system->removeParticle(p);
}
	
	
void lightningBulletFadeParticleCallback(Particle* p, ParticleSystem* system)
{
	//p->mAlpha-=0.08f;
	//if(p->mAlpha <= 0)
		system->removeParticle(p);
}

void lightningGlowParticleCallback(Particle* p, ParticleSystem* system)
{
    LightningWeaponParticleData* data = (LightningWeaponParticleData*)(p->mData);
    
    p->mImage.mCenter = gWorld->getPlayer()->getCenter();
	p->mImage.mSize*=1.4;
	p->mImage.makeDirty();
    
    p->mAlpha-=0.2f;
    if(p->mAlpha <= 0)
    {
        --data->mWeapon->mParticleCount;
        if(data->mWeapon->mParticleCount == 0)
        {
            gAudioLibrary->stopAudioInstance(data->mWeapon->mAudio);
        }            
        
        system->removeParticle(p);
    }
}


}

