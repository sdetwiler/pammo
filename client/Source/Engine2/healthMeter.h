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
    HealthMeter();
    virtual ~HealthMeter();

    virtual uint32_t getDrawPriority() const;
    virtual void draw();

    virtual void setPercent(float percent);

private:
    Image* mHealthDot;
    float mPercent;
    int mMaxDots;
};

} // namespace pammo

#endif // __HEALTHMETER_H__