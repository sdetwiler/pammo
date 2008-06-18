#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "pammo.h"
#include "image.h"
#include "imageEntity.h"

namespace pammo
{

struct Particle;
    // Return false if particle should be expired. True if not.
typedef bool (*ParticleCb)(Particle* p);

bool fireParticleCb(Particle* p);

struct Particle
{
    ParticleCb   mCallback;
    Vector2      mVelocity;
    float        mMass;

    ImageEntity  mImage;
    float        mAlpha;
    Vector2      mEndPosition;
    float        mOldMag;
};


class ParticleSystem
{
public:
    ParticleSystem(uint32_t numParticles);
    virtual ~ParticleSystem();

    void update();
    void draw();

    void initFireParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity);
    void initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity);

protected:
private:

    typedef vector< Particle* > ParticleVector;
    ParticleVector mAvailable;
    ParticleVector mUsed;
};


} // namespace pammo

#endif // __PARTICLESYSTEM_H__