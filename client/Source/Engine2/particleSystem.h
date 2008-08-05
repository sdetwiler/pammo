#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "pammo.h"
#include "imageEntity.h"
#include "view.h"

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

class ParticleSystem //: public View
{
public:
    ParticleSystem(uint32_t numParticles);
    virtual ~ParticleSystem();
   /** 
    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;

    virtual void update();
    virtual void draw();
**/
    // Returns null if no particles are available.
    Particle* addParticle(uint32_t priority);
    Particle* addParticleWithBody(uint32_t priority);
	void removeParticle(Particle* p);

    struct InitFireParticleArgs
    {
        Vector2         initialPosition;
        float           initialRotation;
        Vector2         initialVelocity;
    };
    void initFireParticle(InitFireParticleArgs const& args);

	void initSmokeParticle(Vector2 const& initialPosition, float initialRotation, Vector2 const& initialVelocity);

	void initExplosionParticle(Vector2 const& initialPosition);

    struct InitBallParticleArgs
    {
        Vector2         initialPosition;
        float           initialRotation;
        Vector2         initialVelocity;
        float           maxDistance;
    };
    void initBallParticle(InitBallParticleArgs const& args); 

	void returnParticle(Particle* p);


	class ParticleManager : public View
	{
	public:
		ParticleManager();
		virtual ~ParticleManager();

		void setSystem(ParticleSystem* system);
		void setPriority(ViewPriorities priority);
		virtual uint32_t getDrawPriority() const;
		virtual uint32_t getUpdatePriority() const;

		virtual void draw();
		virtual void update();

		void addParticle(Particle* p);
		void removeParticle(Particle* p);


	private:
		// Pending draw particles.
		Particle* mDrawHead;
		
		// Pending add particles.
		Particle* mAddHead;
		Particle* mAddTail;

		Particle* mRemoveHead;

		// Active particles.
		Particle* mHead;
		Particle* mTail;

		// Owning particle system.
		ParticleSystem* mParticleSystem;
		ViewPriorities mPriority;
	};


	Particle* mFree;
	ParticleManager* mManagers;
	uint32_t mManagerCount;
};

// Particle callback function prototype.
typedef void (*ParticleCb)(Particle* p, ParticleSystem* system);

struct Particle
{
	ParticleSystem::ParticleManager* mManager;
    uint32_t     mSerialNum;        // For debugging to track particles.
    ParticleCb   mCallback;         // Callback to modify particle on each update.
    Body*        mBody;             // Body for physics based particles.

    ImageEntity  mImage;
    float        mAlpha;

	Vector2      mVelocity;         // Velocity of particle.

	Vector2      mStartPosition;
    Vector2      mEndPosition;

	Particle*    mNext;
	Particle*    mPrev;
	Particle*    mRemoveNext;
	Particle*    mDrawNext;
};




} // namespace pammo

#endif // __PARTICLESYSTEM_H__