#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "pammo.h"
#include "image.h"
#include "imageEntity.h"
#include "view.h"

//#include "performanceMonitor.h"

namespace pammo
{

struct Particle;
class ParticleSystem;
    // Return false if particle should be expired. True if not.
typedef bool (*ParticleCb)(Particle* p, ParticleSystem* system);

//typedef void (*ParticleHitCb)(Vehicle* vehicle, void* arg);


bool fireParticleCb(Particle* p, ParticleSystem* system);


struct Particle
{
    uint32_t     mSerialNum;
    ParticleCb   mCallback;             // Callback to modify particle on each update.

    //ParticleHitCb mHitCallback;         // Callback function when hit occurs.
    //void*         mHitCallbackArg;      // Argument to pass to mHitCallback.
    //Vehicle*      mHitVehicle;          // What vehicle will be hit by this particle.

    Vector2      mStartPosition;
    Vector2      mVelocity;             // Velocity of particle.
    float        mMass;                 // Mass of particle.
    float        mRadius;               // JEM Used for debugging of collision sysmte... This is a hack!

    ImageEntity  mImage;
    float        mAlpha;
    Vector2      mEndPosition;
    float        mOldMag;
    bool         mHitsObject;           // Will the particle hit an object.
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

    struct InitFireParticleArgs
    {
        //Vehicle const*  emitter;
        Vector2         initialPosition;
        float           initialRotation;
        Vector2         initialVelocity;
        //ParticleHitCb   hitCallback;
        void*           hitCallbackArg;
    };

    //void removeVehicleTarget(Vehicle* vehicle);

    void initFireParticle(InitFireParticleArgs const& args);
    //void initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity);
    //void initHitParticle(Vector2 const& initialPosition);
    //void initExplosionParticle(Vector2 const& initialPosition);

    //struct InitBallParticleArgs
    //{
    //    Vehicle const*  emitter;
    //    Vector2         initialPosition;
    //    float           initialRotation;
    //    Vector2         initialVelocity;
    //    ParticleHitCb   hitCallback;
    //    void*           hitCallbackArg;
    //    float           maxDistance;
    //};
    //void initBallParticle(InitBallParticleArgs const& args); 

protected:
private:

    typedef vector< Particle* > ParticleVector;
    ParticleVector mAvailable;
    ParticleVector mUsed;

    //PerformanceMonitor* mMonitor;
};


} // namespace pammo

#endif // __PARTICLESYSTEM_H__