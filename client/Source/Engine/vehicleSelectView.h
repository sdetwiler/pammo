#ifndef __VEHICLESELECT_VIEW_H__
#define __VEHICLESELECT_VIEW_H__

#include "pammo.h"
#include "widget.h"

namespace pammo
{

struct Image;

class VehicleSelectView : public View
{
public:
    VehicleSelectView(char const* mapName);
    virtual ~VehicleSelectView();
    
    virtual void init();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update();
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    
private:
    void selectVehicle(uint32_t type);
    
    Image* mBackground;
    
    struct VehicleDesc
    {
        uint32_t mType;
        Vector2 mCenter;
        Vector2 mSize;
        Image* mImage;
    };
    typedef vector< VehicleDesc* > VehicleDescVector;
    VehicleDescVector mVehicles;
    std::string mMapName; // Selected map from map view.
};
    
}

#endif