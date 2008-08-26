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
        Transform2 trans = Transform2::createTranslation(Vector2(5+(i*20), 32));
        trans*= Transform2::createScale(mHealthDot->mSize);
        
        drawImage(mHealthDot, trans, 1.0f);

        ++i;
        numDots -= 1.0;
    }
    if(numDots > 0.0f)
    {
        Transform2 trans = Transform2::createTranslation(Vector2(5+(i*20), 32));
        trans*= Transform2::createScale(mHealthDot->mSize);
        drawImage(mHealthDot, trans, numDots);
    }
}




} // namespace pammo