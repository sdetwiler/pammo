#include "player.h"
#include "world.h"
#include "camera.h"
#include "flameTankVehicle.h"

namespace pammo
{

Player::Player()
{
    mVehicle = NULL;
    mPathManager = NULL;
}

Player::~Player()
{
    if(mVehicle)
        delete mVehicle;
    if(mPathManager)
        delete mPathManager;
}

int Player::init()
{
    int ret;

    mPathManager = new PathManager(this);

    mVehicle = new FlameTankVehicle;
    ret = mVehicle->init();
    if(ret < 0)
        return ret;

    return 0;
}

bool Player::isMoving()
{
    if(!mVehicle)
        return false;

    return mVehicle->isMoving();
}

bool Player::touch(uint32_t count, Touch* touches)
{
    if(mPathManager->touch(count, touches) == true)
        return true;

    return mVehicle->touch(count, touches);
}

uint32_t Player::getTouchPriority() const
{
    return 0;
}

void Player::update()
{
    mVehicle->update();
}

Vector2 const& Player::getCenter() const
{
    return mVehicle->mCenter;
}

void Player::draw()
{
    mVehicle->draw();
}

void Player::setPath(Vector2Vec const& path)
{

    Vector2Vec worldPath;
    for(Vector2Vec::const_iterator i = path.begin(); i!=path.end(); ++i)
    {
        worldPath.push_back(*i);
    }
    mVehicle->setPath(worldPath);
}



}