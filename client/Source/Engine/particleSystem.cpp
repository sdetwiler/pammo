#include "particleSystem.h"

namespace pammo
{

bool fireParticleCb(Particle* p)
{
    p->mImage.mCenter.x += p->mVelocity.x;
    p->mImage.mCenter.y += p->mVelocity.y;
    p->mImage.makeDirty();
    p->mAlpha-=0.085f;
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
    for(ParticleVector::iterator i = mUsed.begin(); i!=mUsed.end(); ++i)
    {
        Particle* p = *i;
        if(p->mCallback(p) == false)
        {
            mAvailable.push_back(*i);
            i = mUsed.erase(i);
        }
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

void ParticleSystem::initFireParticle(Vector2 const& position, float theta)
{
    if(mAvailable.size() == 0)
        return;

    Particle* p = mAvailable.back();
    mAvailable.pop_back();
    mUsed.push_back(p);

    float velocity = 20.0f;
    p->mCallback = fireParticleCb;
    p->mVelocity.x = velocity*cos(theta);
    p->mVelocity.y = velocity*sin(theta);
    p->mMass = 0;
    p->mImage.setImage(openImage("data/materials/flame.png"));
    p->mImage.mCenter = position;
    p->mImage.mRotation = theta;
    p->mAlpha = 1.0f;
}


} // namespace pammo