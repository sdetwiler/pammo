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
    mObserver = NULL;
    mHealth = 100.0f;
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

void Player::setObserver(PlayerObserver* o)
{
    mObserver = o;
    if(mObserver)
        mObserver->onHealthChanged(this, mHealth);
}

void Player::setHealth(float h)
{
    mHealth = h;
}

float Player::getHealth() const
{
    return mHealth;
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
    mVehicle->setPath(path);
/**
    Vector2Vec worldPath;
    for(Vector2Vec::const_iterator i = path.begin(); i!=path.end(); ++i)
    {
        worldPath.push_back(*i);
    }
    mVehicle->setPath(worldPath);
    **/
}



}