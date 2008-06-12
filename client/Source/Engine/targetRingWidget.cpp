#include "targetRingWidget.h"
#include "imageLibrary.h"
#include "world.h"
#include "camera.h"

#include "flameTankVehicle.h"

namespace pammo
{

TargetRingWidget::TargetRingWidget(Vehicle* vehicle)
{
    mVehicle = vehicle;
    mImage = NULL;
    mAngle = 0;
}

TargetRingWidget::~TargetRingWidget()
{
    if(mImage)
        gImageLibrary->unreference(mImage);
}

int TargetRingWidget::init()
{
    mImage = gImageLibrary->reference("data/interface/targetRing.png");
    if(!mImage)
        return -1;

    mHighlightImage = gImageLibrary->reference("data/interface/targetRingHighlight.png");
    if(!mHighlightImage)
        return -1;

    return 0;
}

bool TargetRingWidget::touch(uint32_t count, Touch* touches)
{
    // Always only use first touch point.
    Vector2 loc = gWorld->getCamera()->translateToWorld(touches[0].mLocation);
    float x = mVehicle->mCenter.x - loc.x;
    float y = mVehicle->mCenter.y - loc.y;
    float hyp = sqrt(x*x + y*y);
    dprintf("TargetRingWidget::touch x: %.2f y: %.2f hyp: %.2f\n", x, y, hyp);
    mHighlighted = false;
    // Outside of ring.
    if(hyp > 64.0)
        return false;

    // Inside of ring.
    if(hyp < 48.0f)
        return false;
    dprintf("inside ring\n");
    mHighlighted = true;

    float theta = atan2(y, x);
    mAngle = theta - (90.0f*0.0174532925f);
    
    ((FlameTankVehicle*)mVehicle)->setFireDirection(mAngle);
    
    return true;
}


void TargetRingWidget::draw()
{
    Transform2 trans;
    trans*= Transform2::createTranslation(mVehicle->mCenter);
    trans*= Transform2::createRotation(mAngle);
	trans*= Transform2::createScale(Vector2(2,2));
    Vector2 back;
    back.x = -mVehicle->mCenter.x;
    back.y = -mVehicle->mCenter.y;
    trans*= Transform2::createTranslation(back);
    trans*= mVehicle->getTransform();


    if(mHighlighted)
        drawImage(mHighlightImage, trans, 0.5f);
    else
        drawImage(mImage, trans, 0.5f);
}


} // namespace pamo