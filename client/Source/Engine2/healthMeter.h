#ifndef __HEALTHMETER_H__
#define __HEALTHMETER_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Image;

class HealthMeter : public View
{
public:
    HealthMeter(uint32_t priority);
    virtual ~HealthMeter();

    virtual uint32_t getDrawPriority() const;
    virtual void draw();

    virtual void setPercent(float percent);
    void setGrowDirection(int direction);
    void setBaseLocation(Vector2 baseLocation);

private:
    uint32_t mPriority;
    Image* mHealthDot;
    float mPercent;
    int mMaxDots;
    
    int mGrowDirection;
    Vector2 mBaseLocation;
};

} // namespace pammo

#endif // __HEALTHMETER_H__