#include "vehicle.h"

namespace pammo
{

Vehicle::Vehicle()
{
    mImage = NULL;
    mSpeed = 10.0f;
}

Vehicle::~Vehicle()
{

}

int Vehicle::init()
{
    Image* image = openImage("data/vehicles/flameTank.png");
    if(!image)
    {
        return -1;
    }
    
    mCenter = Vector2(350, 350);

    mImage = image;
    setSize(mImage->mSize);

    mMoving = false;

    return 0;
}

void Vehicle::draw()
{
    drawImage(mImage, getTransform());
}

bool Vehicle::touch(uint32_t count, Touch* touches)
{
    return false;
}

uint32_t Vehicle::getTouchPriority() const
{
    return 0;
}

void Vehicle::update(int delta)
{
    if(mMoving == false)
        return;

    float toTravel = mSpeed;// * (delta/30.0f);

    // How much to translate.
    float theta;
    while(true)
    {
        float dx = (*mCurrTarget).x - mCenter.x;
        float dy = (*mCurrTarget).y - mCenter.y;
        float hyp = sqrt((dx*dx) + (dy*dy));
        theta = atan2(dy, dx);

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


}
