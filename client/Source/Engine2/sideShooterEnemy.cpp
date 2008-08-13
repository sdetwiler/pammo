#include "sideShooterEnemy.h"
#include "world.h"
#include "player.h"
#include "camera.h"
#include "physics.h"
#include "imageLibrary.h"

namespace pammo
{

void sideShooterEnemyInit(Enemy* e, EnemyManager* manager)
{
    e->mEntity.setImage(gImageLibrary->reference("data/vehicles/sideShooter/00.png"));
    e->mDrawCb = sideShooterEnemyDraw;
    e->mUpdateCb = sideShooterEnemyUpdate;
}

void sideShooterEnemyUpdate(Enemy* e, EnemyManager* manager)
{
    e->mController.update();

    Vector2 heading = gWorld->getPlayer()->getCenter() -  e->mBody->mCenter;

    e->mEntity.mRotation = e->mController.mRotation + (float)M_PI/2;
    e->mEntity.mCenter = e->mBody->mCenter;
    e->mEntity.makeDirty();

    float mag = .2f; // TODO: speed.
    float rot = atan2(heading.y, heading.x);
    if(mag < 0.1)
    {
        e->mController.mAcceleration = 0;
    }
    else
    {
        if(rot < 0) 
            rot += (float)M_PI*2;

        e->mController.mAcceleration = mag * e->mBody->mMass * 7;
        e->mController.mRotationTarget = rot;
    }

    e->mEntity.makeDirty();

}

void sideShooterEnemyDraw(Enemy* e, EnemyManager* manager)
{
    gWorld->getCamera()->set();
    
    e->mEntity.draw();
    
    gWorld->getCamera()->unset();
}

} // namespace pammo