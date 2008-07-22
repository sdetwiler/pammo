#include "enemy.h"

#include "world.h"
#include "player.h"
#include "camera.h"
#include "physics.h"

namespace pammo
{

Enemy::Enemy(Vector2 const& position)
{
    mBody = gWorld->getPhysics()->addBody();
    mBody->mCenter = position;
    mBody->mProperties = 1;
    mBody->mDamping = 0.1;
    mBody->mRadius = 20;
    mBody->mMass = 100;
    
    mController = new VehicleController();
    mController->mBody = mBody;
    mController->mRotationDamping = 0.4;
}

Enemy::~Enemy()
{
    delete mBody;
}

void Enemy::update()
{
    mController->update();

    Vector2 heading = gWorld->getPlayer()->getCenter() -  mBody->mCenter;

    mEntity->mRotation = mController->mRotation + M_PI/2;
    mEntity->mCenter = mBody->mCenter;
    mEntity->makeDirty();


    float mag = .2; // TODO: speed.
    float rot = atan2(heading.y, heading.x);
    if(mag < 0.1)
    {
        mController->mAcceleration = 0;
    }
    else
    {
        if(rot < 0) 
            rot += M_PI*2;

        mController->mAcceleration = mag * mBody->mMass * 7;
        mController->mRotationTarget = rot;
    }

    mEntity->makeDirty();
}

void Enemy::draw()
{
    gWorld->getCamera()->set();
    
    mEntity->draw();
    
    gWorld->getCamera()->unset();
}

void Enemy::setCenter(Vector2 center)
{
    mEntity->mCenter = center;
    mEntity->makeDirty();
}


} // namespace pammo 