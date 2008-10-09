
#include "debugScreenView.h"
#include "imageLibrary.h"
#include "drawImage.h"

#include "particleSystem.h"
#include "player.h"
#include "world.h"
#include "physics.h"

namespace pammo
{

struct DebugGooParticleData
{
    int mDirection;
    uint64_t mExpireTime;
};

void debugGooBulletCollisionCallback(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceMe = false;
	response->mBounceThem = false;
	
	doDamage(self, other, Goo, 1.0f);
}
	
void debugGooBulletShapeCollisionCallback(Body* self, Shape* other, Contact* contact, bool* response)
{
    *response = true;
}

void debugGooSprayParticleCallback(Particle* p, ParticleSystem* system)
{
    p->mImage.mSize *= 1.2f;
    if(p->mImage.mSize.x >= 32.0f)
        system->removeParticle(p);

    p->mImage.mCenter+= p->mVelocity;
    p->mImage.makeDirty();

}

void debugGooBulletParticleCallback(Particle* p, ParticleSystem* system)
{
    DebugGooParticleData* particleData = (DebugGooParticleData*)p->mData;

    
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




DebugScreenView::DebugScreenView()
    : View()
{
}

DebugScreenView::~DebugScreenView()
{
}
    

uint32_t DebugScreenView::getDrawPriority() const
{
    return kDebugScreenPriority;
}

uint32_t DebugScreenView::getUpdatePriority() const
{
    return kDebugScreenPriority;
}

void DebugScreenView::update()
{

    for(int i=0; i<5; ++i)
    {
        // Get a particle.
        Particle* p = gWorld->getParticleSystem()->addParticleWithBody(0, false);
        if(!p) return;
        

        // Set basic particle properties.
        p->mCallback = debugGooBulletParticleCallback;
        p->mAlpha = 1.0f;
        
        DebugGooParticleData* particleData = (DebugGooParticleData*)p->mData;
        particleData->mExpireTime = getTime() + 4000000;

	    float velocity = ((rand()%10)/10.0f);
    	
	    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_GOO_00));
        p->mImage.mCenter = gWorld->getPlayer()->mTurretTip;
        p->mImage.makeDirty();

        p->mBody->mProperties = kPlayerBulletCollisionProperties;
        p->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerBarrierCollisionProperties;
        p->mBody->mBodyCallback = debugGooBulletCollisionCallback;
        p->mBody->mShapeCallback = debugGooBulletShapeCollisionCallback;
        p->mBody->mDamping = 0;
        p->mBody->mRadius = 16;
        p->mBody->mMass = 10;
        p->mBody->mCenter = p->mImage.mCenter;
        Vector2 v = Vector2(velocity, 0) * Transform2::createRotation(p->mImage.mRotation);
        p->mBody->mVelocity = v;

    }
}

void DebugScreenView::draw()
{
}

} // namespace pammo

