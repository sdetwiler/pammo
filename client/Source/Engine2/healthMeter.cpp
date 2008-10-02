#include "healthMeter.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

HealthMeter::HealthMeter(uint32_t priority)
{
    mPriority = priority;
    mPercent = 0;
    mMaxDots = 10;
    mHealthDot = gImageLibrary->reference("data/interface/healthDot.png");
    
    mGrowDirection = 1;
    mBaseLocation = Vector2(0, 0);
}

HealthMeter::~HealthMeter()
{

}

uint32_t HealthMeter::getDrawPriority() const
{
    return mPriority;
}

void HealthMeter::setPercent(float percent)
{
    mPercent = percent;
}

void HealthMeter::setGrowDirection(int growDirection)
{
    mGrowDirection = growDirection;
}

void HealthMeter::setBaseLocation(Vector2 baseLocation)
{
    mBaseLocation = baseLocation;
}

void HealthMeter::draw()
{
    int i=0;
    float numDots = mPercent/1000*mMaxDots;
    while(numDots>=1.0f)
    {
        Transform2 trans = Transform2::createTranslation(mBaseLocation + Vector2(mGrowDirection*(i*8), 0));
        trans*= Transform2::createScale(mHealthDot->mSize/2.0f);
        
        drawImage(mHealthDot, trans, 1.0f);

        ++i;
        numDots -= 1.0;
    }
    if(numDots > 0.0f)
    {
        Transform2 trans = Transform2::createTranslation(mBaseLocation + Vector2(mGrowDirection*(i*8), 0));
        trans*= Transform2::createScale(mHealthDot->mSize/2.0f);
        drawImage(mHealthDot, trans, numDots);
    }
}

} // namespace pammo