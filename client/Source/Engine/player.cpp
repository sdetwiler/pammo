#include "player.h"
#include "world.h"
#include "camera.h"
#include "flameTankVehicle.h"
#include "collisionDynamics.h"

namespace pammo
{

Player::Player()
{
    mVehicle = NULL;
    mPathManager = NULL;
    mObserver = NULL;
    mHealth = 100.0f;
    mState = Spawning;
    mSpawnFrame = 0;
    mFrameCount = 0;
}

Player::~Player()
{
    if(mVehicle)
        delete mVehicle;
    if(mPathManager)
        delete mPathManager;
}

int Player::init(Player::Type type)
{
    mType = type;
    mPathManager = new PathManager(this);

    spawn();

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
    if(mObserver)
        mObserver->onHealthChanged(this, mHealth);

    if(mHealth <= 0)
    {
        if(getState() == Alive)
        {
            destroy();
        }
    }
}

void Player::destroy()
{
    for(int i=0; i<5; ++i)
    {
        gWorld->getParticleSystem()->initExplosionParticle(mVehicle->mCenter);
    }

    setState(Destroyed);
    mVehicle->destroy();
    gWorld->getParticleSystem()->removeVehicleTarget(mVehicle);
    gWorld->getCollisionDynamics()->removeVehicle(mVehicle);
    delete mVehicle;
    mVehicle = NULL;
    mSpawnFrame = mFrameCount;
}

void Player::spawn()
{
    int ret;
    setState(Spawning);

    setHealth(100.0f);

    // Wouldn't it be great if this selected from many vehicle types?
    mVehicle = new FlameTankVehicle;
    mVehicle->setObserver(this);
    ret = mVehicle->init();
    if(ret < 0)
    {
        dprintf("Vehicle::init failed %d\n", ret);
        assert(0);
        return;
    }

    // When did spawn occur.
    mSpawnFrame = mFrameCount;

    switch(mType)
    {
    case Local:
        mVehicle->setCollisionBodyMask(mVehicle->getCollisionBodyMask() | LOCALPLAYER);
        break;

    case Remote:
        mVehicle->setCollisionBodyMask(mVehicle->getCollisionBodyMask() | REMOTEPLAYER);
        break;
    }

    gWorld->getCollisionDynamics()->addVehicle(mVehicle);
}

float Player::getHealth() const
{
    return mHealth;
}

Player::State Player::getState() const
{
    return mState;
}

void Player::setState(Player::State state)
{
    mState = state;
}

void Player::onHit(Vehicle* vehicle, float damage)
{
    if(getState() != Alive)
        return;

    float newHealth = getHealth() - damage;
    if(newHealth < 0)
        newHealth = 0;
    setHealth(newHealth);
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

    if(mVehicle)
        return mVehicle->touch(count, touches);

    return false;
}


uint32_t Player::getTouchPriority() const
{
    return 0;
}

void Player::update()
{
    ++mFrameCount;
    switch(getState())
    {
    case Spawning:
        if(mFrameCount - mSpawnFrame > (30 * 3))
        {
            setState(Alive);
        }
        mVehicle->update();
        break;
    
    case Alive:
        mVehicle->update();
        break;

    case Destroyed:
        if(mFrameCount - mSpawnFrame > (30 * 3))
        {
            spawn();
        }
    }
}

Vector2 const& Player::getCenter() const
{
    return mVehicle->mCenter;
}

void Player::draw()
{
    if(mVehicle)
        mVehicle->draw();
}

void Player::setPath(Vector2Vec const& path)
{
    if(mVehicle)
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