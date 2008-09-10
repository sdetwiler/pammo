#include "healthMeter.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

HealthMeter::HealthMeter()
{
    mPercent = 0;
    mMaxDots = 10;
    mHealthDot = gImageLibrary->reference("data/interface/healthDot.png");
}

HealthMeter::~HealthMeter()
{

}

uint32_t HealthMeter::getDrawPriority() const
{
    return kHealthMeterPriority;
}

void HealthMeter::setPercent(float percent)
{
    mPercent = percent;
}

void HealthMeter::draw()
{
    int i=0;
    float numDots = mPercent/1000*mMaxDots;
    while(numDots>=1.0f)
    {
        Transform2 trans = Transform2::createTranslation(Vector2((480/2) + 32 + (i*8), 16));
        trans*= Transform2::createScale(mHealthDot->mSize/2.0f);
        
        drawImage(mHealthDot, trans, 1.0f);

        ++i;
        numDots -= 1.0;
    }
    if(numDots > 0.0f)
    {
        Transform2 trans = Transform2::createTranslation(Vector2((480/2) + 32 + (i*8), 16));
        trans*= Transform2::createScale(mHealthDot->mSize/2.0f);
        drawImage(mHealthDot, trans, 0.5f);
    }
}




} // namespace pammo