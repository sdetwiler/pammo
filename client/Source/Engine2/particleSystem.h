#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "pammo.h"
#include "imageEntity.h"
#include "view.h"

//#include "performanceMonitor.h"

namespace pammo
{

enum ParticleType
{
    Fire,
    Lightning
};

struct Body;
struct Particle;
class ParticleSystem;

// Particle callback function prototype.
typedef void (*ParticleCb)(Particle* p, ParticleSystem* system);

struct Particle
{
    uint32_t     mSerialNum;        // For debugging to track particles.
    ParticleCb   mCallback;         // Callback to modify particle on each update.
    Body*        mBody;             // Body for physics based particles.

    ImageEntity  mImage;
    float        mAlpha;

	Vector2      mVelocity;         // Velocity of particle.

	Vector2      mStartPosition;
    Vector2      mEndPosition;
};

class ParticleSystem : public View
{
public:
    ParticleSystem(uint32_t numParticles);
    virtual ~ParticleSystem();
    
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;

    void update();
    void draw();

    // Returns null if no particles are available.
    // JEM Should accept some sort of priority, so that bullets can pull the rug out from smoke particles.
    Particle* addParticle();
    Particle* addParticleWithBody();
	void removeParticle(Particle* p);




    struct InitFireParticleArgs
    {
        //Vehicle const*  emitter;
        Vector2         initialPosition;
        float           initialRotation;
        Vector2         initialVelocity;
        //ParticleHitCb   hitCallback;
//        void*           hitCallbackArg;
    };
    
    void initFireParticle(InitFireParticleArgs const& args);
    void initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity);
    //void initHitParticle(Vector2 const& initialPosition);
    void initExplosionParticle(Vector2 const& initialPosition);

    struct InitBallParticleArgs
    {
        Vector2         initialPosition;
        float           initialRotation;
        Vector2         initialVelocity;
        float           maxDistance;
    };
    void initBallParticle(InitBallParticleArgs const& args); 

protected:
private:

    typedef vector< Particle* > ParticleVector;
    ParticleVector mAvailable;
    ParticleVector mUsed;
	ParticleVector mRemoved;

    //PerformanceMonitor* mMonitor;
};


} // namespace pammo

#endif // __PARTICLESYSTEM_H__