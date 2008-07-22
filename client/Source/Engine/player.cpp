#include "player.h"
#include "world.h"
#include "camera.h"
#include "flameTankVehicle.h"
#include "trebuchetVehicle.h"
#include "collisionDynamics.h"

namespace pammo
{

Player::Player()
{
    mVehicle = NULL;
    mVehicleType = 0;
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
        mPathManager->destroy();
}

int Player::init(Player::Type type)
{
    mType = type;
    
    if(type == Local)
        mPathManager = new PathManager(this);
    else
        mPathManager = 0;

    spawn();

    return 0;
}

void Player::setObserver(PlayerObserver* o)
{
    mObserver = o;
    if(mObserver)
        mObserver->onHealthChanged(this, mHealth);
}

void Player::setVehicleType(uint32_t type)
{
    mVehicleType = type;
}

uint32_t Player::getVehicleType() const
{
    return mVehicleType;
}

Vehicle* Player::getVehicle() const
{
    return mVehicle;
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

    
    if(mType == Local)
    {
        switch(mVehicleType)
        {
        case VEHICLE_FLAMETANK:
            mVehicle = new FlameTankVehicle;
            ((FlameTankVehicle*)mVehicle)->setTargetRingState(true);
            break;

        case VEHICLE_TREBUCHET:
            mVehicle = new TrebuchetVehicle;
            break;
        
        default:
            dprintf("Unknown vehicle type %u\n", mVehicleType);
            assert(0);
        }
    }

    else
    {
        if(false && rand()%2)
        {
            mVehicle = new FlameTankVehicle;
            mVehicleType = VEHICLE_FLAMETANK;
        }
        else
        {
            mVehicle = new TrebuchetVehicle;
            mVehicleType = VEHICLE_TREBUCHET;
        }
    }

    mVehicle->setObserver(this);
    ret = mVehicle->init();
    if(ret < 0)
    {
        dprintf("Vehicle::init failed %d\n", ret);
        assert(0);
        return;
    }
    
    // Choose a random spawn location.
    mVehicle->mCenter = gWorld->getRandomSpawnPoint();

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
    // JEM: You and I? We shall live forever in the fortress of our hearts.
    //return;
    // Another time my love. For this evening is the one of despaire.

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
    if(gWorld->isZoomedOut() == true)
        return false;

    if(mVehicle)
        return mVehicle->touch(count, touches);

    return false;
}


uint32_t Player::getTouchPriority() const
{
    return 0;
}

void Player::npcUpdate()
{
    // Expensive, so only do on occasion.
    if(rand()%5)
    {
        return;
    }

    Vehicle* closest = gWorld->getCollisionDynamics()->getClosestVehicle(mVehicle);
    if(!closest)
        return;

    Vector2 targetVector = mVehicle->mCenter - closest->mCenter;
    float theta = atan2(targetVector.y, targetVector.x);
    theta-=(90.0f*0.0174532925f);

    switch(mVehicleType)
    {
    case VEHICLE_FLAMETANK:
        ((FlameTankVehicle*)mVehicle)->setFireDirection(theta);
        break;
    case VEHICLE_TREBUCHET:
        float noise = (rand()%8)-4;
        ((TrebuchetVehicle*)mVehicle)->setFireDirection(theta-((noise+180.0f)*0.0174532925f));
        if(!(rand()%3))
        {
            float mag = magnitude(targetVector);
            if(mag > 200)
                mag = 200;
            else if(mag < 1)
                mag = 5;

            dprintf("mag: %.2f", mag);
            ((TrebuchetVehicle*)mVehicle)->setFireMagnitude(magnitude(targetVector));
            ((TrebuchetVehicle*)mVehicle)->fire();
        }

        break;
    }
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
        if(mType== Remote)
        {
            npcUpdate();
        }
        break;
    
    case Alive:
        mVehicle->update();
        if(mType== Remote)
        {
            npcUpdate();
        }
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

void Player::setCenter(Vector2 const& center)
{
    mVehicle->mCenter = center;
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


void Player::setSwarmPoint(Vector2 const& point)
{
    mSwarmPoint = point;
}

Vector2 const& Player::getSwarmPoint()
{
    return mSwarmPoint;
}


}