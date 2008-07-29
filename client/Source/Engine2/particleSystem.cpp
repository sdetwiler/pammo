#include "particleSystem.h"
#include "imageLibrary.h"
#include "world.h"
#include "camera.h"
#include "physics.h"

#include "player.h"
#include "enemyManager.h"

#include <algorithm>

namespace pammo
{

void doDamage(Body* self, Body* other, ParticleType type, float damage)
{

	if(self->mCollideProperties & other->mProperties)
	{
		if(other->mProperties & kPlayerCollisionProperties)
		{
			((Player*)(other->mUserArg))->damage(type, damage);
		}
		else if(other->mProperties & kEnemyCollisionProperties)
		{
			Enemy* e = (Enemy*)other->mUserArg;
			if(e->mDestroyed)
				return;

			if(e->mDamageCb)
				e->mDamageCb(e, type, damage);
		}
	}
}

void fireParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= 1.05f;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
        gWorld->getPhysics()->removeBody(p->mBody);
		system->removeParticle(p);
		return;
    }

    p->mAlpha-=0.075f;
}


void fireCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

	doDamage(self, other, Fire, 10.0f);
}

void ballCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
    response->mBounceThem = true;
    response->mBounceMe = true;

	Particle* p = (Particle*)(self->mUserArg);
    gWorld->getPhysics()->removeBody(self);
    gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
	gWorld->getParticleSystem()->removeParticle(p);

//	doDamage(self, other, Ball, 100.0f);
}

void ballParticleCb(Particle* p, ParticleSystem* system)
{
    if(magnitude(p->mEndPosition - p->mStartPosition) < magnitude(p->mBody->mCenter - p->mStartPosition))
    {
        gWorld->getPhysics()->removeBody(p->mBody);
		system->removeParticle(p);
		return;
    }

    p->mImage.mCenter = p->mBody->mCenter;
    float distance = magnitude(p->mImage.mCenter - p->mStartPosition)/magnitude(p->mEndPosition - p->mStartPosition);
    float x = (distance-0.5f)*1.5f;
    float y = (-(x*x))+1;
    p->mImage.mSize = 16 * y;
    p->mImage.makeDirty();
}

void smokeParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;
    p->mImage.makeDirty();
    p->mImage.mSize *= 1.03f;
    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
	{
        gWorld->getPhysics()->removeBody(p->mBody);
		system->removeParticle(p);
	}
}

void explosionParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mSize*=1.07f;
    p->mImage.mRotation+= ((float)(rand()%10)-5.0f)/100.0f;
    p->mImage.makeDirty();

    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
	{
        //gWorld->getPhysics()->removeBody(p->mBody);
		system->removeParticle(p);
	}
}

ParticleSystem::ParticleSystem(uint32_t numParticles)
    : View()
{
    //mMonitor = new PerformanceMonitor("Used Particles", 30);

    for(uint32_t i=0; i<numParticles; i++)
    {
        Particle* p = new Particle;
        p->mSerialNum = i;
        mAvailable.push_back(p);
    }
}

ParticleSystem::~ParticleSystem()
{
    for(ParticleVector::iterator i = mAvailable.begin(); i!=mAvailable.end(); ++i)
    {
        delete *i;
    }

    for(ParticleVector::iterator i = mUsed.begin(); i!=mUsed.end(); ++i)
    {
        delete *i;
    }

    for(ParticleVector::iterator i = mRemoved.begin(); i!=mRemoved.end(); ++i)
    {
        delete *i;
    }
}

uint32_t ParticleSystem::getUpdatePriority() const
{
    return kParticlePriority;
}

uint32_t ParticleSystem::getDrawPriority() const
{
    return kParticlePriority;
}

void ParticleSystem::removeParticle(Particle* p)
{
	mRemoved.push_back(p);
	mUsed.erase(std::find(mUsed.begin(), mUsed.end(), p));
}

void ParticleSystem::update()
{
    //mMonitor->addSample(mUsed.size());

	for(uint32_t i=0; i<mRemoved.size(); ++i)
	{
		Particle* p = mRemoved[i];
		gImageLibrary->unreference(p->mImage.getImage());
		mAvailable.push_back(p);
	}
	mRemoved.clear();

    // Iterator over each particle, making its callback.
    for(uint32_t i = 0; i<mUsed.size(); ++i)
    {
        Particle* p = mUsed[i];
        p->mCallback(p, this);
    }
}

void ParticleSystem::draw()
{
    gWorld->getCamera()->set();

    for(ParticleVector::iterator i = mUsed.begin(); i!=mUsed.end(); ++i)
    {
        //if((*i)->mRadius != 0) continue;
        
        (*i)->mImage.mAlpha = (*i)->mAlpha;
        (*i)->mImage.draw();
    }
    
    gWorld->getCamera()->unset();
}

void ParticleSystem::initFireParticle(InitFireParticleArgs const& args)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);
        
    // Properties about fire particles.
    float velocity = 300.0f;

    // Set basic particle properties.
    p->mCallback = fireParticleCb;
    p->mAlpha = 1.0f;

    float f = 8.0f;
    float r = 1.0f/f - ((rand()%100)/(f*50));
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.mCenter = args.initialPosition;
    p->mImage.mRotation = args.initialRotation + r;
    p->mImage.makeDirty();
    
    // Get a body.
    p->mBody = gWorld->getPhysics()->addBody();
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 20;
    p->mBody->mMass = 10;
    p->mBody->mCenter = args.initialPosition;
    p->mBody->mVelocity = args.initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(args.initialRotation+r);
    p->mBody->mBodyCallback = fireCollisionCb;
    p->mBody->mUserArg = p;
}

void ParticleSystem::initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);
        
    // Properties about smoke particles.
    float velocity = 5.0f;

    // Set basic particle properties.
    p->mCallback = smokeParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/smoke00.png"));
    p->mImage.mCenter = initialPosition;
    p->mImage.mRotation = initialRotation;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(initialRotation);
}

void ParticleSystem::initExplosionParticle(Vector2 const& initialPosition)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);

    // Set basic particle properties.
    p->mCallback = explosionParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/explosion00.png"));
    p->mImage.mCenter = initialPosition + Vector2((float)(rand()%20), (float)(rand()%20));
    p->mImage.makeDirty();
}

void ParticleSystem::initBallParticle(InitBallParticleArgs const& args)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);
        
    // Properties about ball particles.
    float velocity = 300.0f;
    float particleRadius = 8.0f;

    // Set basic particle properties.
    p->mCallback = ballParticleCb;
    p->mAlpha = 1.0f;
    p->mStartPosition = args.initialPosition;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/ball.png"));
    p->mImage.mCenter = args.initialPosition;
    p->mImage.mRotation = args.initialRotation;
    p->mImage.makeDirty();

    // Get a body.
    p->mBody = gWorld->getPhysics()->addBody();
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kEnemyCollisionProperties | kPlayerCollisionProperties;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 8;
    p->mBody->mMass = 50;
    p->mBody->mCenter = args.initialPosition;
    p->mBody->mVelocity = args.initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(args.initialRotation);
    p->mBody->mBodyCallback = ballCollisionCb;
    p->mBody->mUserArg = p;

    // Calculate unblocked end position. Start position plus maxDistance rotated for direction.
    p->mEndPosition = args.initialPosition + Vector2(args.maxDistance, 0) * Transform2::createRotation(args.initialRotation);
}




} // namespace pammo
