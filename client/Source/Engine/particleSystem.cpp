#include "particleSystem.h"
#include "imageLibrary.h"
#include "world.h"
#include "collisionMap.h"
#include "camera.h"

namespace pammo
{

bool fireParticleCb(Particle* p)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;
    p->mImage.makeDirty();

    float mag = magnitude(p->mImage.mCenter - p->mEndPosition);
   // dprintf("mag: %.2f", mag);
    // Really close to dest.
    if(mag < 10)
        return false;

    // Passed it.
    if(p->mOldMag && (mag > p->mOldMag))
        return false;
    //p->mAlpha-=0.05f;
//    if(p->mAlpha<=0)
//       return false;

    p->mOldMag = mag;
    return true;
}

bool smokeParticleCb(Particle* p)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;
    p->mImage.makeDirty();
    p->mAlpha-=0.05f;
    if(p->mAlpha<=0)
        return false;
    return true;
}

ParticleSystem::ParticleSystem(uint32_t numParticles)
{
    for(uint32_t i=0; i<numParticles; i++)
    {
        mAvailable.push_back(new Particle);
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

void ParticleSystem::update()
{
    typedef vector<int> IntVector;
    IntVector removeParticles;
    
    // Iterator over each particle, making its callback. If callback returns true, puts its index into the removeParticles list.
    for(uint32_t i = 0; i<mUsed.size(); ++i)
    {
        Particle* p = mUsed[i];
        if(p->mCallback(p) == false)
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
    for(ParticleVector::iterator i = mUsed.begin(); i!=mUsed.end(); ++i)
    {
        (*i)->mImage.mAlpha = (*i)->mAlpha;
        (*i)->mImage.draw();
    }
}

void ParticleSystem::initFireParticle(Transform2 const& transform, Vector2 const& initialVelocity)
{
    if(mAvailable.size() == 0)
        return;

    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);

    Vector2 start;
    start = start * transform;

    p->mCallback = fireParticleCb;
    p->mOldMag = 0;
    float velocity = 10.0f;
//    p->mVelocity = normalize(velocity * transform);
    p->mVelocity.x = velocity*transform[0];
    p->mVelocity.y = velocity*transform[1];
    //p->mVelocity += initialVelocity;

    p->mMass = 0;
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame.png"));
    p->mImage.setTransform(transform);
    p->mAlpha = 1.0f;
    
    float maxDistance = 40.0f;
    // Is this sane? dv/dt in a unit vector direction.
    p->mEndPosition = start + (normalize(start)*(p->mVelocity*20.0f));

    float particleRadius = 1.0f;
    Vector2 hit;
    dprintf("World\n  start: [%.2f, %.2f]\n  end:  [%.2f, %.2f]", start.x, start.y, p->mEndPosition.x, p->mEndPosition.y);
    
    if(gWorld->getCollisionMap()->raycast(start, p->mEndPosition, particleRadius, hit))
        p->mEndPosition = hit;

    dprintf("hit:  [%.2f, %.2f]\n", p->mEndPosition.x, p->mEndPosition.y);

}


void ParticleSystem::initSmokeParticle(Transform2 const& transform, Vector2 const& initialVelocity)
{
    if(mAvailable.size() == 0)
        return;

    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);

    float velocity = 4.0f;
    p->mCallback = smokeParticleCb;

    p->mVelocity.x = velocity*transform[0];
    p->mVelocity.y = velocity*transform[1];

    p->mVelocity += initialVelocity;
    p->mMass = 0;
    p->mImage.setImage(gImageLibrary->reference("data/particles/smoke00.png"));
    p->mImage.setTransform(transform);
    p->mAlpha = 1.0f;
}

} // namespace pammo
