#ifndef __CREDITSSCREEN_VIEW_H__
#define __CREDITSSCREEN_VIEW_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

struct Image;

class CreditsScreenView : public View
{
public:
    CreditsScreenView();
    virtual ~CreditsScreenView();
    
    virtual void update();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    
    virtual uint32_t getTouchPriority() const;
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getUpdatePriority() const;

private:
    
    Image* mBackground;
    
    //struct VehicleDesc
    //{
    //    uint32_t mType;
    //    Vector2 mCenter;
    //    Vector2 mSize;
    //    Image* mImage;
    //};
    //typedef vector< VehicleDesc* > VehicleDescVector;
    //VehicleDescVector mVehicles;
    //std::string mMapName; // Selected map from map view.
};
  
}

#endif