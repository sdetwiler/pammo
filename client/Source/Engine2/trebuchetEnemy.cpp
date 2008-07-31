#include "world.h"
#include "player.h"
#include "physics.h"
#include "camera.h"
#include "imageLibrary.h"
#include "particleSystem.h"

#include "enemyManager.h"
#include "trebuchetEnemy.h"

namespace pammo
{

void trebuchetEnemyInit(Enemy* e, EnemyManager* manager)
{
    e->mEntity = new ImageEntity(gImageLibrary->reference("data/vehicles/trebuchet/00.png"));
    e->mDrawCb = trebuchetEnemyDraw;
    e->mUpdateCb = trebuchetEnemyUpdate;
	e->mDamageCb = trebuchetEnemyDamage;
	e->mDestroyCb = trebuchetEnemyDestroy;
	e->mHealth = 100.0f;
}

void trebuchetEnemyFire(Enemy* e, float distance)
{
    Vector2 baseVelocity;
    float rad = 32.0f; // Distance from center of vehicle to end of trebuchet.
    float rot = e->mEntity->mRotation - (float)M_PI/2;
    
    // Calculate center. Vehicle center plus arm rotated for direction.
    Vector2 center = e->mEntity->mCenter + Vector2(rad, 0) * Transform2::createRotation(rot);

    ParticleSystem::InitBallParticleArgs args;
    args.initialPosition = center;
    args.initialRotation = rot;
    args.initialVelocity = e->mBody->mVelocity;
    args.maxDistance = distance;

    gWorld->getParticleSystem()->initBallParticle(args);
}

void trebuchetEnemyUpdate(Enemy* e, EnemyManager* manager)
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
            trebuchetEnemyFire(e, mag);
    }
    else
    {
        e->mController.mAcceleration = speed * e->mBody->mMass * 7;
    }

    e->mEntity->makeDirty();
}


void trebuchetEnemyDraw(Enemy* e, EnemyManager* manager)
{
    e->mEntity->draw();
}

void trebuchetEnemyDestroy(Enemy* e, EnemyManager* manager)
{
	dprintf("%p You sunk my trebuchet!");
	gWorld->getParticleSystem()->initExplosionParticle(e->mBody->mCenter);
	gImageLibrary->unreference(e->mEntity->getImage());
	delete e->mEntity;

}

void trebuchetEnemyDamage(Enemy* e, ParticleType type, float amount)
{
	e->mHealth-=amount;

	if(e->mHealth <=0)
	{
		gWorld->getEnemyManager()->destroyEnemy(e);
	}
}


} // namespace pammo