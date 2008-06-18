#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "pammo.h"
#include "image.h"
#include "imageEntity.h"

namespace pammo
{

struct Particle;
class ParticleSystem;
    // Return false if particle should be expired. True if not.
typedef bool (*ParticleCb)(Particle* p, ParticleSystem* system);

bool fireParticleCb(Particle* p, ParticleSystem* system);

struct Particle
{
    ParticleCb   mCallback;
    Vector2      mVelocity;
    float        mMass;

    ImageEntity  mImage;
    float        mAlpha;
    Vector2      mEndPosition;
    float        mOldMag;
    bool         mHitsObject;
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
    void initHitParticle(Vector2 const& initialPosition);

protected:
private:

    typedef vector< Particle* > ParticleVector;
    ParticleVector mAvailable;
    ParticleVector mUsed;
};


} // namespace pammo

#endif // __PARTICLESYSTEM_H__