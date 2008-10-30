#ifndef __BARMETER__H__
#define __BARMETER_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

class Image;

class BarMeter : public View
{
public:
    BarMeter(uint32_t priority);
    virtual ~BarMeter();

    virtual uint32_t getUpdatePriority() const;
    virtual uint32_t getDrawPriority() const;
    virtual void update();
    virtual void draw();

    virtual void setTargetPercent(float percent);
    virtual void setCurrentPercent(float percent);
    void setGrowDirection(int direction);
    void setBaseLocation(Vector2 baseLocation);
    void setColor(float r, float g, float b);

private:
    uint32_t mPriority;
    float mTargetPercent;
    float mCurrentPercent;
    
    float mR, mG, mB;
    
    Image* mMeterBar;
    int mGrowDirection;
    Vector2 mBaseLocation;
};

} // namespace pammo

#endif // __BarMeter_H__