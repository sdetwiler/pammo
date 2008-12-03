#include "gooWeapon.h"

#include "world.h"
#include "player.h"
#include "physics.h"
#include "particleSystem.h"
#include "imageLibrary.h"
#include "enemyManager.h"

const uint32_t kGooNormalRadius = 16;

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
    mParticleCount = 0;
    mAudio = gAudioLibrary->getAudioInstance(AUDIO_GOOGUN);
    mIcon.setImage(gImageLibrary->getImage(INTERFACE_ICON_WEAPON_GOO));
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
    enum Mode
    {
        kModeFlying,
        kModeExpanding,
        kModeContracting
    };
    
    Mode mMode;
    uint32_t mRemainingFrames;
    float mSize;
    float mRotateDirection;
    float mAlphaVelocity;
    float mSizeFactor;
    GooWeapon* mWeapon;
};

void GooWeapon::fire()
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(0, false);
    if(!p) return;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    // Set basic particle properties.
    p->mCallback = gooBulletParticleCallback;
    p->mAlpha = 0.0f;
    
    // Set particle properties.
    GooParticleData* particleData = (GooParticleData*)p->mData;
    particleData->mWeapon = this;
    particleData->mSize = 0.4;
    particleData->mAlphaVelocity = 0.3;
    particleData->mSizeFactor = 1.2;
    particleData->mRemainingFrames = 60;
    if(rand()%2) particleData->mRotateDirection = -1;
    else particleData->mRotateDirection = 1;

    // Choose some numbers.
    float f = 10.0f;
    float r = 1.0f/f - ((rand()%100)/(f*90)) ;
    float initialRotation = player->mTurret.mRotation - M_PI/2.0f;
    
    // Setup image.
	//int i=rand()%3;
	float velocity = 160;
	velocity += ((rand()%40)/40.0f);
	
	p->mImage.setImage(gImageLibrary->getImage(PARTICLE_GOO_00 + rand()%PARTICLE_GOO_COUNT));
    p->mImage.mCenter = player->mTurretTip + Vector2(4, 0) * Transform2::createRotation(initialRotation+r);
    p->mImage.mRotation = initialRotation+r;
    p->mImage.makeDirty();
        
    // Properties about goo bullet particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerBarrierCollisionProperties;
    p->mBody->mBodyCallback = gooBulletCollisionCallback;
    p->mBody->mShapeCallback = gooBulletShapeCollisionCallback;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = kGooNormalRadius * particleData->mSize;
    p->mBody->mMass = 10;
    p->mBody->mCenter = p->mImage.mCenter;
    p->mBody->mVelocity = player->mBody->mVelocity + Vector2(velocity, 0) * Transform2::createRotation(p->mImage.mRotation);

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

void gooBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    if(rand() % 4 == 0)
        gWorld->getParticleSystem()->initSmokeParticle(contact->mContactPoint, 0.0f, Vector2(0,0), 15);
        
	response->mBounceMe = false;
	response->mBounceThem = false;
    
	doDamage(self, other, Goo, 3.0f);
}
	
void gooBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    if(rand() % 8 == 0)
        gWorld->getParticleSystem()->initSmokeParticle(contact->mContactPoint, 0.0f, Vector2(0,0), 5);
    
    *response = true;
    //*response = false;
    self->mVelocity = Vector2(0, 0);
    contact->mSeparatingVelocity = 0;
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
    GooParticleData* data = (GooParticleData*)p->mData;
    
    if(data->mAlphaVelocity)
    {
        p->mAlpha += data->mAlphaVelocity;
        if(p->mAlpha < 0) p->mAlpha = 0;
        if(p->mAlpha > 1) p->mAlpha = 1;
    }
    
    data->mSize *= data->mSizeFactor;

    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += (data->mRotateDirection*0.015f);
    p->mImage.mSize =  p->mImage.getImage()->mSize * data->mSize;
    p->mImage.makeDirty();
    
    p->mBody->mRadius = kGooNormalRadius * data->mSize;
    gWorld->getPhysics()->resortBody(p->mBody);
    
    switch(data->mMode)
    {
        case GooParticleData::kModeFlying:
            --data->mRemainingFrames;
            if(data->mSize >= 1)
            {
                data->mAlphaVelocity = -0.01;
                data->mSizeFactor = (1.005 + ((float)(rand()%100)/10000.0f));
                p->mBody->mDamping = 0.3;
                
                --data->mWeapon->mParticleCount;
                if(data->mWeapon->mParticleCount == 0)
                {
                    gAudioLibrary->stopAudioInstance(data->mWeapon->mAudio);
                }            
                
                
                data->mMode = GooParticleData::kModeExpanding;
            }
            break;
        case GooParticleData::kModeExpanding:
            data->mSizeFactor = (1.005 + ((float)(rand()%100)/10000.0f));
            --data->mRemainingFrames;
            if(data->mRemainingFrames == 0)
            {
                data->mAlphaVelocity = -0.03f;
                data->mSizeFactor = 0.95;
                
                data->mMode = GooParticleData::kModeContracting;
            }
            break;
        case GooParticleData::kModeContracting:
            if(p->mAlpha == 0)
            {
                system->removeParticle(p);
            }
            break;
        default:
            assert(0);
    }
}
	

}

