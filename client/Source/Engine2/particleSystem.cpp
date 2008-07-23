#include "particleSystem.h"
#include "imageLibrary.h"
#include "world.h"
#include "camera.h"
#include "physics.h"

namespace pammo
{

bool fireParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter = p->mBody->mCenter;
    p->mImage.mRotation += 0.08f;
    p->mImage.mSize *= 1.05;
    p->mImage.makeDirty();
    
    if(p->mAlpha <= 0)
    {
        gWorld->getPhysics()->removeBody(p->mBody);
        return false;
    }
    
    p->mAlpha-=0.05f;
    return true;
}


bool ballParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;

    float distance = magnitude(p->mImage.mCenter - p->mStartPosition)/magnitude(p->mEndPosition - p->mStartPosition);
    //dprintf("Distance: %.2f", distance);

    float x = (distance-0.5)*1.5;
    float y = (-(x*x))+1;
    //dprintf("Scale: %.2f", y);
    p->mImage.mSize = 16 * y;
    
    p->mImage.makeDirty();

    float mag = magnitude(p->mImage.mCenter - p->mEndPosition);
   // dprintf("mag: %.2f", mag);
    // Really close to dest.
    if(mag < 10)
    {
        if(p->mHitsObject)
        {
            //system->initExplosionParticle(p->mEndPosition);
        }
        else
        {
            //system->initHitParticle(p->mEndPosition);
        }
        return false;
    }

    // Passed it.
    if(p->mOldMag && (mag > p->mOldMag))
    {
        if(p->mHitsObject)
        {
           // system->initExplosionParticle(p->mEndPosition);
        }
        else
        {
            //system->initHitParticle(p->mEndPosition);
        }

        return false;
    }

    p->mOldMag = mag;
    return true;
}

bool smokeParticleCb(Particle* p, ParticleSystem* system)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;
    p->mImage.makeDirty();
    p->mImage.mSize *= 1.03;
    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
        return false;
    return true;
}
#if 0

bool hitParticleCb(Particle* p, ParticleSystem* system)
{
    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
        return false;
    return true;
}

bool explosionParticleCb(Particle* p, ParticleSystem* system)
{

    p->mImage.mSize*=1.07;
    p->mImage.mRotation+= ((float)(rand()%10)-5.0f)/100.0f;
    p->mImage.makeDirty();

    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
        return false;
    return true;
}
#endif

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
}

uint32_t ParticleSystem::getUpdatePriority() const
{
    return kParticlePriority;
}

uint32_t ParticleSystem::getDrawPriority() const
{
    return kParticlePriority;
}

void ParticleSystem::update()
{
    //mMonitor->addSample(mUsed.size());

    typedef vector<int> IntVector;
    IntVector removeParticles;
    
    // Iterator over each particle, making its callback. If callback returns true, puts its index into the removeParticles list.
    for(uint32_t i = 0; i<mUsed.size(); ++i)
    {
        Particle* p = mUsed[i];
        if(p->mCallback(p, this) == false)
        {
            removeParticles.push_back(i);
        }
    }
    
    // Now iterate over remove list and the particles.
    for(uint32_t i=0; i < removeParticles.size(); ++i)
    {
        // Get a reference the particle. Every iteratation is going to remove one particle.
        // Which means the indexes stored in removeParticles are wrong.
        // To adjust, subtract from the stored index the number of particels we have already removed.
        uint32_t indexInUsed = removeParticles[i] - i;
        Particle* p = mUsed[indexInUsed];
        gImageLibrary->unreference(p->mImage.getImage());
        mUsed.erase(mUsed.begin() + indexInUsed);
        mAvailable.push_back(p);
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

    float f = 8.0;
    float r = 1.0/f - ((rand()%100)/(f*50));
    
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
    p->mBody->mMass = 50;
    p->mBody->mCenter = args.initialPosition;
    p->mBody->mVelocity = args.initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(args.initialRotation+r);
}

#if 0

void ParticleSystem::initFireParticle(InitFireParticleArgs const& args)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);
        
    // Properties about fire particles.
    float velocity = 10.0f;
    float maxDistance = 150.0f;
    float particleRadius = 10.0f;

    // Set basic particle properties.
    p->mCallback = fireParticleCb;
    p->mOldMag = 0;
    p->mMass = 0;
    p->mRadius = particleRadius;
    p->mHitsObject = false;
    p->mAlpha = 1.0f;
    p->mHitCallback = args.hitCallback;
    p->mHitCallbackArg = args.hitCallbackArg;
    p->mHitVehicle = NULL;

    float f = 8.0;
    float r = 1.0/f - ((rand()%100)/(f*50)) ;
    
    // Setup image.
    //p->mImage.setImage(gImageLibrary->reference("data/particles/flame2-01.png"));
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.mCenter = args.initialPosition;
    p->mImage.mRotation = args.initialRotation + r;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = args.initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(args.initialRotation+r);
    
    // Calculate unblocked end position. Start position plus maxDistance rotated for direction.
    p->mEndPosition = args.initialPosition + Vector2(maxDistance, 0) * Transform2::createRotation(args.initialRotation+r);

    //dprintf("World\n  start: [%.2f, %.2f]\n  end:  [%.2f, %.2f]", initialPosition.x, initialPosition.y, p->mEndPosition.x, p->mEndPosition.y);
    
    // Collide with collision map.
    CollisionMap::RaycastResult mapResult;
    gWorld->getCollisionMap()->raycast(args.initialPosition, p->mEndPosition, particleRadius, mapResult);
    
    // Collide with collision dynamics.
    CollisionDynamics::RaycastResult dynamicsResult;
    uint32_t flags;
    if(args.emitter->getCollisionBodyMask() & LOCALPLAYER)
        flags = REMOTEPLAYER;
    else
        flags = LOCALPLAYER;

    gWorld->getCollisionDynamics()->raycast(args.initialPosition, p->mEndPosition, particleRadius, velocity, flags, dynamicsResult);
    
    // Determine which was closer.
    if(mapResult.mHit && (!dynamicsResult.mHit || mapResult.mDistance < dynamicsResult.mDistance))
    {
        p->mEndPosition = mapResult.mPosition;
        p->mHitsObject = true;
    }
    else if(dynamicsResult.mHit)
    {
        p->mEndPosition = dynamicsResult.mPosition;
        p->mHitVehicle = dynamicsResult.mBody->mVehicle;
        p->mHitsObject = true;
    }

    //dprintf("hit:  [%.2f, %.2f]", p->mEndPosition.x, p->mEndPosition.y);

}
#endif

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
   // p->mMass = 0;
//    p->mRadius = 0;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/smoke00.png"));
    p->mImage.mCenter = initialPosition;
    p->mImage.mRotation = initialRotation;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(initialRotation);
}
#if 0
void ParticleSystem::initHitParticle(Vector2 const& initialPosition)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);

    // Set basic particle properties.
    p->mCallback = hitParticleCb;
    p->mMass = 0;
    p->mRadius = 0;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/smoke00.png"));
    p->mImage.mCenter = initialPosition + Vector2(rand()%10, rand()%10);
    p->mImage.makeDirty();
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
    p->mMass = 0;
    p->mRadius = 0;
    p->mAlpha = 1.0f;

    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/explosion00.png"));
    p->mImage.mCenter = initialPosition + Vector2(rand()%20, rand()%20);
    p->mImage.makeDirty();
}
#endif

void ParticleSystem::initBallParticle(InitBallParticleArgs const& args)
{
    if(mAvailable.size() == 0)
        return;

    // Grab a particle.
    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);
        
    // Properties about ball particles.
    float velocity = 10.0f;
    float particleRadius = 8.0f;

    // Set basic particle properties.
    p->mCallback = ballParticleCb;
    p->mOldMag = 0;
    p->mHitsObject = false;
    p->mAlpha = 1.0f;
    p->mStartPosition = args.initialPosition;
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/ball.png"));
    p->mImage.mCenter = args.initialPosition;
    p->mImage.mRotation = args.initialRotation;
    p->mImage.makeDirty();
    
    // Setup velocity. InitialVelocity from vehicle plus particle speed rotated for direction.
    p->mVelocity = args.initialVelocity + Vector2(velocity, 0) * Transform2::createRotation(args.initialRotation);
    
    // Calculate unblocked end position. Start position plus maxDistance rotated for direction.
    p->mEndPosition = args.initialPosition + Vector2(args.maxDistance, 0) * Transform2::createRotation(args.initialRotation);

    //dprintf("World\n  start: [%.2f, %.2f]\n  end:  [%.2f, %.2f]", initialPosition.x, initialPosition.y, p->mEndPosition.x, p->mEndPosition.y);
    
}




} // namespace pammo
