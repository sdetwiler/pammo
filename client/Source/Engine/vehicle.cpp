#include "game.h"
#include "vehicle.h"
#include "imageLibrary.h"
#include "animationEntity.h"

namespace pammo
{

Vehicle::Vehicle()
{
    mSpeed = 5.0f;
    mOldTheta = 0;
    mMoving = false;
    mCollisionBodyMask = 0;
    mCollisionBodyRadius = 16;
    mObserver = 0;
}

Vehicle::~Vehicle()
{

}

int Vehicle::init()
{
    return 0;
}

bool Vehicle::isMoving()
{
    return mMoving;
}

void Vehicle::setCollisionBodyMask(uint32_t mask)
{
    mCollisionBodyMask = mask;
}

uint32_t Vehicle::getCollisionBodyMask() const
{
    return mCollisionBodyMask;
}

float Vehicle::getCollisionBodyRadius() const
{
    return mCollisionBodyRadius;
}

float Vehicle::getSpeed() const
{
    if(mMoving) return mSpeed;
    else return 0;
}

float Vehicle::getDirection() const
{
    return mOldTheta;
}

void Vehicle::draw()
{
    mAnimation.setTransform(getTransform());
    mAnimation.draw();
}

bool Vehicle::touch(uint32_t count, Touch* touches)
{
    return false;
}

uint32_t Vehicle::getTouchPriority() const
{
    return 0;
}

void Vehicle::update()
{
    if(mMoving == false)
        return;

    float toTravel = mSpeed;

    // How much to translate.
    float theta;
    while(true)
    {
        float dx = (*mCurrTarget).x - mCenter.x;
        float dy = (*mCurrTarget).y - mCenter.y;
        float hyp = sqrt((dx*dx) + (dy*dy));
        theta = atan2(dy, dx);

        mOldTheta = theta;
        

//        dprintf("dx: %.2f dy: %.2f hyp: %.2f c.x: %.2f c.y: %.2f t.x: %.2f t.y: %.2f theta: %.2f\n", dx, dy, hyp, mCenter.x, mCenter.y, (*mCurrTarget).x, (*mCurrTarget).y, theta);

        if(hyp < toTravel)
        {
            ++mCurrTarget;
            if(mCurrTarget == mPath.end())
            {
                mMoving = false;
                mCenter.x -= cos(theta)*hyp;
                mCenter.y -= sin(theta)*hyp;
                break;
            }
        }
        else if(hyp > toTravel)
        {
            mCenter.x += cos(theta)*toTravel;
            mCenter.y += sin(theta)*toTravel;
            break;
        }
        else
        {
            mCenter = *mCurrTarget;
            ++mCurrTarget;
            if(mCurrTarget == mPath.end())
            {
                mMoving = false;
            }
            break;
        }
    }

    mRotation = theta + (90.0*0.0174532925);

    makeDirty();
//    mAnimation.setTransform(getTransform());
}

void Vehicle::setPath(Vector2Vec const& points)
{
    mPath = points;

    if(mPath.size() > 2)
    {
        mMoving = true;
        mCurrTarget = mPath.begin();
        //mPosition = *mCurrTarget;
       // ++mCurrTarget;
    }
    else
    {
        mMoving = false;
    }
}


void Vehicle::setObserver(VehicleObserver* o)
{
    mObserver = o;
}

VehicleObserver* Vehicle::getObserver()
{
    return mObserver;
}


}
