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
    gWorld->getParticleSystem()->initExplosionParticle(self->mCenter);
	gWorld->getParticleSystem()->removeParticle(p);

//	doDamage(self, other, Ball, 100.0f);
}

void ballParticleCb(Particle* p, ParticleSystem* system)
{
    if(magnitude(p->mEndPosition - p->mStartPosition) < magnitude(p->mBody->mCenter - p->mStartPosition))
    {
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
		system->removeParticle(p);
	}
}

void rubbleParticleCb(Particle* p, ParticleSystem* system)
{
    //p->mImage.mSize*=1.07f;
    //p->mImage.mRotation+= ((float)(rand()%10)-5.0f)/100.0f;
    p->mImage.makeDirty();

    p->mImage.mSize*=.99f;
    p->mAlpha-=0.01f;
    if(p->mAlpha<=0)
	{
		system->removeParticle(p);
	}
}



////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(uint32_t numParticles)
 //   : View()
{
	mManagerCount = 4;

	mManagers = new ParticleManager*[mManagerCount];
	for(uint32_t i=0; i<mManagerCount; ++i)
	{
        mManagers[i] = new ParticleManager;
        mManagers[i]->setSystem(this);
		switch(i)
		{
		case 0:
			mManagers[i]->setPriority(kParticle0Priority);
			break;
		case 1:
			mManagers[i]->setPriority(kParticle1Priority);
			break;
		case 2:
			mManagers[i]->setPriority(kParticle2Priority);
			break;
		case 3:
			mManagers[i]->setPriority(kParticle3Priority);
			break;
		}
	}

	mFree = NULL;
	for(uint32_t i=0; i<numParticles; ++i)
	{
		Particle* p = new Particle;
		
		p->mNext = mFree;
		mFree = p;
	}
}

ParticleSystem::~ParticleSystem()
{
    uint32_t i=0;
    while(mFree)
    {
        Particle* p = mFree->mNext;
        delete mFree;
        mFree = p;
        ++i;
    }

    delete[] mManagers;
}

void ParticleSystem::reset()
{
    for(uint32_t i=0; i<mManagerCount; ++i)
    {
        mManagers[i]->reset();
    }

    //uint32_t i=0;
    //Particle* p = mFree;
    //while(p)
    //{
    //    ++i;
    //    dprintf("%p %u", p, i);     
    //    p = p->mNext;
    //}

    //dprintf("ParticleSystem::reset now has %u particles", i);
}

void ParticleSystem::enable()
{
    for(uint32_t i=0; i<mManagerCount; ++i)
    {
        mManagers[i]->enableAll();
    }
}

void ParticleSystem::disable()
{
    for(uint32_t i=0; i<mManagerCount; ++i)
    {
        mManagers[i]->disableAll();
    }
}


void ParticleSystem::destroy()
{
    for(uint32_t i=0; i<mManagerCount; ++i)
    {
        mManagers[i]->destroy();
    }
}

Particle* ParticleSystem::addParticle(uint32_t priority)
{
	if(priority > (mManagerCount-1))
		return NULL;

    // Grab a particle.
	Particle* p;
	if(mFree == NULL)
	{
		return NULL;
	}

	p = mFree;
	mFree = mFree->mNext;
    memset(p, 0, sizeof(Particle));
    
	mManagers[priority]->addParticle(p);

//    dprintf("addParticle %p", p);
    return p;
}

void ParticleSystem::returnParticle(Particle* p)
{
	p->mNext = mFree;
	mFree = p;
//    dprintf("returnParticle %p", p);
}

Particle* ParticleSystem::addParticleWithBody(uint32_t priority)
{
    if(priority > (mManagerCount-1))
		return NULL;

    Body* body = gWorld->getPhysics()->addBody();
    if(!body) return NULL;

	// Grab a particle.
	Particle* p;
	if(mFree == NULL)
	{
		return NULL;
	}
	p = mFree;
	mFree = mFree->mNext;

    memset(p, 0, sizeof(Particle));
    p->mBody = body;
	body->mUserArg = p;
    
	mManagers[priority]->addParticle(p);
        
    return p;
}

void ParticleSystem::removeParticle(Particle* p)
{
	p->mCallback = NULL;
	p->mManager->removeParticle(p);
}

void ParticleSystem::initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity)
{
    // Grab a particle.
    Particle* p = addParticle(1);
    if(!p) return;
        
    // Properties about smoke particles.

    // Set basic particle properties.
    p->mCallback = smokeParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/smoke00.png"));
    p->mImage.mCenter = initialPosition;
    p->mImage.mRotation = initialRotation;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = initialVelocity * Transform2::createRotation(initialRotation);
}

void ParticleSystem::initExplosionParticle(Vector2 const& initialPosition)
{
    // Grab a particle.
    Particle* p = addParticle(3);
    if(!p) return;

    // Set basic particle properties.
    p->mCallback = explosionParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/explosion00.png"));
    p->mImage.mCenter = initialPosition + Vector2((float)(rand()%20), (float)(rand()%20));
    p->mImage.makeDirty();
}

void ParticleSystem::initRubbleParticle(Vector2 const& initialPosition)
{
    // Grab a particle.
    Particle* p = addParticle(2);
    if(!p) return;

    // Set basic particle properties.
    p->mCallback = rubbleParticleCb;
    p->mAlpha = 0.8f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/rubble00.png"));
    //p->mImage.mSize = Vector2(2.0f, 2.0f);
    p->mImage.mCenter = initialPosition;// + Vector2((float)(rand()%20), (float)(rand()%20));
    p->mImage.makeDirty();
}


void ParticleSystem::initBallParticle(InitBallParticleArgs const& args)
{
    // Grab a particle.
    Particle* p = addParticleWithBody(3);
    if(!p) return;
        
    // Properties about ball particles.
    float velocity = 300.0f;

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
    p->mBody->mProperties = kEnemyBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kPlayerBarrierCollisionProperties;
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


//////////////////////////////////////////////////////////


ParticleSystem::ParticleManager::ParticleManager() 
	: View()
{
	mParticleSystem = NULL;
	mDrawHead = NULL;
	mAddHead = NULL;
	mAddTail = NULL;
	mRemoveHead = NULL;
	mHead = NULL;
	mTail = NULL;
	mPriority = kParticle0Priority;
}

ParticleSystem::ParticleManager::~ParticleManager()
{
}

void ParticleSystem::ParticleManager::reset()
{
    Particle* p;
    Particle* next;
    // Remove all pending add particles.
    p = mAddHead;
    while(p)
    {
        next = p->mNext;
        mParticleSystem->returnParticle(p);
        p = next;
    }
    mAddHead = NULL;

    // Remove all current particles.
    p = mTail;
    while(p)
    {
        next = p->mPrev;
        mParticleSystem->returnParticle(p);
        p = next;
    }
    mHead = NULL;
    mTail = NULL;
    mRemoveHead = NULL;
}

void ParticleSystem::ParticleManager::setSystem(ParticleSystem* particleSystem)
{
	mParticleSystem = particleSystem;
}

void ParticleSystem::ParticleManager::setPriority(ViewPriorities priority)
{
	mPriority = priority;
}

void ParticleSystem::ParticleManager::draw()
{
	gWorld->getCamera()->set();
	while(mDrawHead)
	{
        mDrawHead->mImage.mAlpha = mDrawHead->mAlpha;
        mDrawHead->mImage.draw();
		mDrawHead = mDrawHead->mDrawNext;
	}
    gWorld->getCamera()->unset();
}

void ParticleSystem::ParticleManager::addParticle(Particle* p)
{
	p->mManager = this;
	if(mAddTail)
	{
		mAddTail->mNext = p;
	}
	else
	{
		mAddHead = p;
	}

	p->mPrev = mAddTail;
	mAddTail = p;
    p->mNext = NULL;
}

void ParticleSystem::ParticleManager::removeParticle(Particle* p)
{
    p->mCallback = NULL;
	if(p->mBody)
		gWorld->getPhysics()->removeBody(p->mBody);
	p->mRemoveNext = mRemoveHead;
	mRemoveHead = p;
}

void ParticleSystem::ParticleManager::update()
{
	// Remove all pending remove items.
	Particle* curr = mRemoveHead;
	while(curr)
	{
		if(curr->mNext)
        {
			curr->mNext->mPrev = curr->mPrev;
        }
        else
			mTail = curr->mPrev;

		if(curr->mPrev)
			curr->mPrev->mNext = curr->mNext;
		else
			mHead = curr->mNext;

		// Return to free pool.
		mParticleSystem->returnParticle(curr);

		curr = curr->mRemoveNext;
	}
	mRemoveHead = NULL;

	// Add all new particles to the end of the active list.
	if(mAddHead)
	{
		if(mTail)
		{
			mTail->mNext = mAddHead;
			mAddHead->mPrev = mTail;
		}
		else
			mHead = mAddHead;
	
		mTail = mAddTail;
		mAddHead = NULL;
		mAddTail = NULL;
	}

    static const float screenRadius = (320.0f * 320.0f) + (480.0f * 480.0f);
    
    uint32_t toDraw=0;
    uint32_t toSkip=0;
	// Update in reverse order to preserve draw order based on insertion order.
	curr = mTail;
	while(curr)
	{
        // Update if callback exists. Callback will be NULL if particle was removed.
		if(curr->mCallback)
		{
			curr->mCallback(curr, mParticleSystem);

            // check if should draw.
            float particleRadius;
            if(curr->mImage.mSize.x > curr->mImage.mSize.y)
                particleRadius = curr->mImage.mSize.x;
            else
                particleRadius = curr->mImage.mSize.y;

            particleRadius*=particleRadius;

            // Center to center.
            float x = gWorld->getCamera()->mCenter.x - curr->mImage.mCenter.x;
            x*=x;
            float y = gWorld->getCamera()->mCenter.y - curr->mImage.mCenter.y;
            y*=y;
            
            if(((x+y) - particleRadius) - screenRadius <=0)
            {
                ++toDraw;
                curr->mDrawNext = mDrawHead;
                mDrawHead = curr;
            }
            else
            {
                ++toSkip;
            }

		}

        curr = curr->mPrev;
	}

    //dprintf("draw %u. cull %u", toDraw, toSkip);
}


uint32_t ParticleSystem::ParticleManager::getDrawPriority() const
{
    return mPriority;
}

uint32_t ParticleSystem::ParticleManager::getUpdatePriority() const
{
    return mPriority;
}

} // namespace pammo
