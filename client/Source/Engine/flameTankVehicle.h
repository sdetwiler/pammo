#ifndef __FLAMETANKVEHICLE_H__
#define __FLAMETANKVEHICLE_H__

#include "vehicle.h"

namespace pammo
{

class FlameTankVehicle : 
    public Vehicle
{
public:
    FlameTankVehicle(World* world);
    virtual ~FlameTankVehicle();

    virtual int init();
    virtual void draw();
    virtual void update(int delta);


protected:
    Image** mImages;
    int mCurrImageIndex;
private:
};

} // namespace pammo

#endif // 