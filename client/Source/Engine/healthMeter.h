#ifndef __HEALTHMETER_H__
#define __HEALTHMETER_H__

#include "game.h"
#include "pammo.h"
#include "image.h"
#include "player.h"

namespace pammo
{

class HealthMeter : 
    public PlayerObserver
{
public:
    HealthMeter();
    virtual ~HealthMeter();

    virtual void draw();
    virtual void init();

    virtual void onHealthChanged(Player* player, float health);

protected:

private:
    Image* mHealthDot;
    float mPercent;
    int mMaxDots;
};

} // namespace pammo

#endif // __HEALTHMETER_H__