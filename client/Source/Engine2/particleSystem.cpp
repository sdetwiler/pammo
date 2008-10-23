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

struct ExplosionParticleData
{
    float mRotation;
};

void explosionParticleCb(Particle* p, ParticleSystem* system)
{
    ExplosionParticleData* data = (ExplosionParticleData*)p->mData;
    p->mImage.mSize*=1.09f;
    p->mImage.mRotation+= data->mRotation;
    p->mImage.makeDirty();

    p->mAlpha-=0.07f;
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

#ifdef PROFILE
    mParticleCount = numParticles;
    mFreeCount = mParticleCount;
    mDrawCount = 0;
    mCullCount = 0;
#endif
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

#ifdef PROFILE
void ParticleSystem::dumpStats()
{
    dprintf("Particle System:   total   used   draw   cull");
    dprintf("                  % 6u % 6u % 6u % 6u\n",
        mParticleCount,
        mParticleCount-mFreeCount,
        mDrawCount,
        mCullCount
        );

}
#endif

Particle* ParticleSystem::addParticle(uint32_t priority, bool optional)
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
    p->mOptional = optional;
	mFree = mFree->mNext;
    memset(p, 0, sizeof(Particle));
	mManagers[priority]->addParticle(p);

#ifdef PROFILE
    --mFreeCount;
#endif

    return p;
}

void ParticleSystem::returnParticle(Particle* p)
{
    p->mNext = mFree;
	mFree = p;

#ifdef PROFILE
    ++mFreeCount;
#endif
}

Particle* ParticleSystem::addParticleWithBody(uint32_t priority, bool optional)
{
    if(priority > (mManagerCount-1))
		return NULL;

    Body* body = gWorld->getPhysics()->addBody();
    if(!body)
        return NULL;

    Particle* p = addParticle(priority, optional);
    if(!p)
    {
        gWorld->getPhysics()->removeBody(body);
        return NULL;
    }

    p->mBody = body;
	body->mUserArg = p;
    
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
    Particle* p = addParticle(2, true);
    if(!p) return;
        
    // Properties about smoke particles.

    // Set basic particle properties.
    p->mCallback = smokeParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_SMOKE_00));
    p->mImage.mCenter = initialPosition;
    p->mImage.mRotation = initialRotation;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = initialVelocity * Transform2::createRotation(initialRotation);
}

void ParticleSystem::initExplosionParticle(Vector2 const& initialPosition)
{
    // Grab a particle.
    Particle* p = addParticle(3, false);
    if(!p) return;

    ExplosionParticleData* data = (ExplosionParticleData*)p->mData;

    data->mRotation = .05f;
    if(rand()%2)
        data->mRotation*= -1.0f;

    // Set basic particle properties.
    p->mCallback = explosionParticleCb;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_EXPLOSION_00 + rand()%PARTICLE_EXPLOSION_COUNT));
    p->mImage.mCenter = initialPosition + Vector2((float)(rand()%20), (float)(rand()%20));
    p->mImage.makeDirty();
}

void ParticleSystem::initRubbleParticle(Vector2 const& initialPosition)
{
    // Grab a particle.
    Particle* p = addParticle(2, true);
    if(!p) return;

    // Set basic particle properties.
    p->mCallback = rubbleParticleCb;
    p->mAlpha = 0.8f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_RUBBLE_00));
    //p->mImage.mSize = Vector2(2.0f, 2.0f);
    p->mImage.mCenter = initialPosition;// + Vector2((float)(rand()%20), (float)(rand()%20));
    p->mImage.makeDirty();
}


void ParticleSystem::initBallParticle(InitBallParticleArgs const& args)
{
    // Grab a particle.
    Particle* p = addParticleWithBody(3, false);
    if(!p) return;
        
    // Properties about ball particles.
    float velocity = 300.0f;

    // Set basic particle properties.
    p->mCallback = ballParticleCb;
    p->mAlpha = 1.0f;
    p->mStartPosition = args.initialPosition;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->getImage(PARTICLE_BALL_00));
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

#ifdef PROFILE
    mLastDrawCount = 0;
    mLastCullCount = 0;
#endif
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
        next = p->mAddNext;
        removeParticle(p);
        p = next;
    }
    mAddHead = NULL;
    mAddTail = NULL;

    // Remove all current particles.
    p = mHead;
    while(p)
    {
        next = p->mNext;
        removeParticle(p);
        p = next;
    }
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
    // If already in the add list, don't add it again.
    if(p->mToAdd)
        return;
    p->mToAdd = true;

	p->mManager = this;
    if(mAddTail)
	{
		mAddTail->mAddNext = p;
	}
	else
	{
		mAddHead = p;
	}

	mAddTail = p;
    p->mAddNext = NULL;
}

void ParticleSystem::ParticleManager::removeParticle(Particle* p)
{
    // If already in remove list, don't add it again.
    if(p->mToRemove)
        return;

    p->mToRemove = true;

    p->mCallback = NULL;
	if(p->mBody)
    {
        gWorld->getPhysics()->removeBody(p->mBody);
    }
	p->mRemoveNext = mRemoveHead;
	mRemoveHead = p;
}

void ParticleSystem::ParticleManager::update()
{
	// Add all new particles to the end of the active list.

    while(mAddHead)
    {
        if(mTail)
        {
            mTail->mNext = mAddHead;
        }
        else
        {
            mHead = mAddHead;
        }
        mAddHead->mPrev = mTail;
        mTail = mAddHead;

        mAddHead->mToAdd = false;
        mAddHead = mAddHead->mAddNext;
    }
    mAddTail = NULL;

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
        curr->mToRemove = false;
        mParticleSystem->returnParticle(curr);

		curr = curr->mRemoveNext;
	}
	mRemoveHead = NULL;
    
    Camera* camera = gWorld->getCamera();
    float cameraLeft = camera->mCenter.x - camera->mSize.x/2;
    float cameraTop = camera->mCenter.y - camera->mSize.y/2;
    float cameraRight = camera->mCenter.x + camera->mSize.x/2;
    float cameraBottom = camera->mCenter.y + camera->mSize.y/2;
    
#ifdef PROFILE
    uint32_t toDraw=0;
    uint32_t toCull=0;
#endif
    // Update in reverse order to preserve draw order based on insertion order.
	curr = mTail;
	while(curr)
	{
        // Update if callback exists. Callback will be NULL if particle was removed.
		if(curr->mCallback)
		{
			curr->mCallback(curr, mParticleSystem);
            
            float sx = curr->mImage.mSize.x/2;
            float sy = curr->mImage.mSize.y/2;
            
            float cx = curr->mImage.mCenter.x;
            float cy = curr->mImage.mCenter.y;
            
            if(    cx + sx > cameraLeft  && cy + sy > cameraTop
                && cx - sx < cameraRight && cy - sy < cameraBottom)
            {
#ifdef PROFILE
                ++toDraw;
#endif
                curr->mDrawNext = mDrawHead;
                mDrawHead = curr;
            }
#ifdef PROFILE
            else
            {
                ++toCull;
            }
#endif
		}

        curr = curr->mPrev;
	}

#ifdef PROFILE
    mParticleSystem->mDrawCount-= mLastDrawCount;
    mParticleSystem->mDrawCount+= toDraw;

    mParticleSystem->mCullCount-= mLastCullCount;
    mParticleSystem->mCullCount+= toCull;

    mLastDrawCount = toDraw;
    mLastCullCount = toCull;
#endif
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
