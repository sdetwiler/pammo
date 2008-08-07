#include "world.h"
#include "player.h"
#include "physics.h"
#include "camera.h"
#include "imageLibrary.h"
#include "particleSystem.h"

#include "enemyManager.h"
#include "flameTankEnemy.h"

namespace pammo
{
void flameTankEnemySurroundUpdate(Enemy* e, EnemyManager* manager);


void flameTankEnemyFireParticleCb(Particle* p, ParticleSystem* system)
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

    p->mAlpha-=0.085f;
}

void flameTankEnemyFireCollisionCb(Body* self, Body* other, Contact* contact, ContactResponse* response)
{
	response->mBounceThem = true;
	response->mBounceMe = true;

	doDamage(self, other, Fire, 10.0f);
}

void flameTankEnemyInitFireParticle(Enemy* e)
{
    // Get a particle.
    Particle* p = gWorld->getParticleSystem()->addParticleWithBody(2);
    if(!p) return;
    
    // Set basic particle properties.
    p->mCallback = flameTankEnemyFireParticleCb;
    p->mAlpha = 1.0f;
    
    // Choose some numbers.
    Vector2 fireDirection = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float f = 8.0;
    float r = atan2(fireDirection.y, fireDirection.x) + 1.0/f - ((rand()%100)/(f*50));
    if(r > M_PI*2.0)
        r-=(M_PI*2.0);
    
    // Setup image.
    p->mImage.setImage(gImageLibrary->reference("data/particles/flame01.png"));
    p->mImage.mCenter = e->mBody->mCenter + Vector2(30.0f,0.0f)*Transform2::createRotation(r);
    p->mImage.mRotation = r;
    p->mImage.makeDirty();
        
    // Properties about fire particles.
    p->mBody->mProperties = kPlayerBulletCollisionProperties;
    p->mBody->mCollideProperties = kPlayerCollisionProperties | kBarrierCollisionProperties;
    p->mBody->mBodyCallback = flameTankEnemyFireCollisionCb;
    p->mBody->mDamping = 0;
    p->mBody->mRadius = 15;
    p->mBody->mMass = 5;
    p->mBody->mCenter = p->mImage.mCenter;
    p->mBody->mVelocity = e->mBody->mVelocity + Vector2(300, 0) * Transform2::createRotation(r);
}



void flameTankEnemyInit(Enemy* e, EnemyManager* manager)
{
    e->mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/flameTank5/00.png"));
    e->mDrawCb = flameTankEnemyDraw;
    e->mUpdateCb = flameTankEnemySurroundUpdate;
	e->mDamageCb = flameTankEnemyDamage;
	e->mHealth = 100.0f;
}

void flameTankEnemyFire(Enemy* e, float distance)
{
    Vector2 baseVelocity;
    float rad = 32.0f; // Distance from center of vehicle to end of flameTank.
    float rot = e->mEntity->mRotation - (float)M_PI/2;
    
    // Calculate center. Vehicle center plus arm rotated for direction.
    Vector2 center = e->mEntity->mCenter + Vector2(rad, 0) * Transform2::createRotation(rot);

    ParticleSystem::InitFireParticleArgs args;
    args.initialPosition = center;
    args.initialRotation = rot;
    args.initialVelocity = e->mBody->mVelocity;

    flameTankEnemyInitFireParticle(e);
}


void flameTankEnemySurroundUpdate(Enemy* e, EnemyManager* manager)
{
    e->mController.update();

    e->mEntity->mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity->mCenter = e->mBody->mCenter;
    e->mEntity->makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float mag = magnitude(heading);
    float speed = .5f;
    float rot = atan2(heading.y, heading.x);
    flameTankEnemyFire(e, mag);

    if(mag < 200)
    {
        //e->mController.mAcceleration = 0;
        // Maybe fire.
//        if(!(rand()%30))
            flameTankEnemyFire(e, mag);

        rot+=(1.57f/1.5f);

    }
    else
    {
        e->mController.mAcceleration = speed * e->mBody->mMass * 7;
    }
    if(rot < 0) 
        rot += (float)M_PI*2;
    else if(rot > (float)M_PI*2)
        rot -= (float)M_PI*2;

    e->mController.mRotationTarget = rot;
    e->mEntity->makeDirty();
}

void flameTankEnemyUpdate(Enemy* e, EnemyManager* manager)
{
    e->mController.update();

    e->mEntity->mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity->mCenter = e->mBody->mCenter;
    e->mEntity->makeDirty();

    Vector2 heading = gWorld->getPlayer()->getCenter() - e->mBody->mCenter;
    float mag = magnitude(heading);
    float speed = .5f;
    float rot = atan2(heading.y, heading.x);
    if(rot < 0) 
        rot += (float)M_PI*2;
    e->mController.mRotationTarget = rot;

    if(mag < 200)
    {
        e->mController.mAcceleration = 0;
        // Maybe fire.
        if(!(rand()%30))
            flameTankEnemyFire(e, mag);
    }
    else
    {
        e->mController.mAcceleration = speed * e->mBody->mMass * 7;
    }

    e->mEntity->makeDirty();
}


void flameTankEnemyDraw(Enemy* e, EnemyManager* manager)
{
    e->mEntity->draw();
}

void flameTankEnemyDamage(Enemy* e, ParticleType type, float amount)
{
	e->mHealth-=amount;

	if(e->mHealth <=0)
	{
		dprintf("%p You sunk my flameTank!", e);
		gWorld->getParticleSystem()->initExplosionParticle(e->mBody->mCenter);
		gImageLibrary->unreference(e->mEntity->getImage());
		gWorld->getEnemyManager()->removeEnemy(e);
		e->mDamageCb = NULL;
	}
}


} // namespace pammo