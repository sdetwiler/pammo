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

    return 0;
}

bool TargetRingWidget::touch(uint32_t count, Touch* touches)
{
    // Always only use first touch point.
    Vector2 loc = gWorld->getCamera()->translateToWorld(touches[0].mLocation);
    float x = mVehicle->mCenter.x - loc.x;
    float y = mVehicle->mCenter.y - loc.y;
    float hyp = sqrt(x*x + y*y);

    // Outside of ring.
    if(hyp > 128.0f)
        return false;

    // Inside of ring.
    if(hyp < 96.0f)
        return false;

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
    Vector2 back;
    back.x = -mVehicle->mCenter.x;
    back.y = -mVehicle->mCenter.y;
    trans*= Transform2::createTranslation(back);
    trans*= mVehicle->getTransform();
    drawImage(mImage, trans, 0.5f);
}


} // namespace pammo