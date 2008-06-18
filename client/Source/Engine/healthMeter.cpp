#include "healthMeter.h"
#include "image.h"
#include "transform2.h"

namespace pammo
{
HealthMeter::HealthMeter()
{
    mPercent = 100;
    mMaxDots = 10;
    mHealthDot = NULL;
}

HealthMeter::~HealthMeter()
{

}

void HealthMeter::init()
{
    mHealthDot = openImage("data/interface/healthDot.png");
    if(!mHealthDot)
        return;

}

void HealthMeter::onHealthChanged(Player* player, float health)
{
    mPercent = health;
}

void HealthMeter::draw()
{
//void drawImage(Image* image, Transform2 const& transform, float alpha=1.0f);
    Transform2 trans;
    
    int i=0;
    float numDots = mPercent/mMaxDots;
    while(numDots>1.0f)
    {
        drawImage(mHealthDot, trans, 1.0f);

        trans = Transform2::createTranslation(Vector2(Vector2(5+(i*20), 5)));
        trans*= Transform2::createScale(mHealthDot->mSize);

        ++i;

        numDots-=1.0;
    }
    if(numDots > 0.0f)
    {
        drawImage(mHealthDot, trans, numDots);
    }
}




} // namespace pammo