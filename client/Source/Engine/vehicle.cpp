#include "vehicle.h"

namespace pammo
{

Vehicle::Vehicle()
{
    mImage = NULL;
}

Vehicle::~Vehicle()
{

}

int Vehicle::init()
{
    Image* image = openImage("flameTank.png");
    if(!image)
    {
        return -1;
    }

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


    float dx = mCenter.x - (*mCurrTarget).x;
    float dy = mCenter.y - (*mCurrTarget).y;

    float theta = atan(dy/dx);
    mRotation = theta-(90.0*0.0174532925);

    mCenter = *mCurrTarget;
    ++mCurrTarget;
    if(mCurrTarget == mPath.end())
    {
        mMoving = false;
    }
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
