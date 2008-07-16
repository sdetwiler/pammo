#include "magnitudeWidget.h"
#include "imageLibrary.h"
#include "world.h"
#include "camera.h"

#include "trebuchetVehicle.h"

namespace pammo
{

MagnitudeWidget::MagnitudeWidget(Vehicle* vehicle)
{
    mVehicle = vehicle;
    mImage = NULL;
    mAngle = 0;
    mMagnitude = 0;
    mTracking = false;
}

MagnitudeWidget::~MagnitudeWidget()
{
    if(mImage)
        gImageLibrary->unreference(mImage);
}

int MagnitudeWidget::init()
{
    mImage = gImageLibrary->reference("data/interface/magnitude.png");
    if(!mImage)
        return -1;

    return 0;
}

bool MagnitudeWidget::touch(uint32_t count, Touch* touches)
{
    if(count < 1)
        return false;

    // Always only use first touch point.
    Vector2 loc = gWorld->getCamera()->translateToWorld(touches[0].mLocation);
    float x = mVehicle->mCenter.x - loc.x;
    float y = mVehicle->mCenter.y - loc.y;
    float hyp = sqrt(x*x + y*y);

    if(touches[0].mPhase == Touch::PhaseBegin)
    {
        // Check if touch is on vehicle.
        // Outside of vehicle. 
        if(hyp > 42.0)
            return false;
        
        mTracking = true;
        return true;
    }

    if(!mTracking)
        return false;

    float theta = atan2(y, x);

    // Move or End
    mMagnitude = hyp;
    mAngle = theta - (90.0f*0.0174532925f);
    ((TrebuchetVehicle*)mVehicle)->setFireDirection(mAngle);
    ((TrebuchetVehicle*)mVehicle)->setFireMagnitude(mMagnitude);

    if(touches[0].mPhase == Touch::PhaseEnd)
    {
        ((TrebuchetVehicle*)mVehicle)->fire();
        mTracking = false;
        mMagnitude = 1;
    }

    return true;
}


void MagnitudeWidget::draw()
{
    if(!mTracking)
        return;

    // Cache x,y for faster typing.
    float x = mImage->mSize.x;
    float y = mImage->mSize.y;

    // Create transform, cribbed from entity.cpp transform setup. Position about center, scale and rotate appropriately.
    // This probably should just use an imageEntity.
    Transform2 trans;
    trans *= Transform2::createTranslation(mVehicle->mCenter);
    trans *= Transform2::createRotation(mAngle);
	trans *= Transform2::createScale(Vector2(1, mMagnitude/8.0f));
    trans *= Transform2::createTranslation(Vector2(-x/2, -y/2));
	trans *= Transform2::createScale(Vector2(x, y));
    
    drawImage(mImage, trans, 0.5f);
}


} // namespace pamo