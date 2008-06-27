
#include "vehicleSelectView.h"
#include "world.h"
#include "image.h"
#include "imageLibrary.h"

#include "flameTankVehicle.h"
#include "trebuchetVehicle.h"

namespace pammo
{

VehicleSelectView::VehicleSelectView()
{
    gGame->queueInitable(this);
    
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/VehicleSelectBackground.png");
    
    float border = 12;
    Vector2 pos(border, 92);

    VehicleDesc* vehicle = new VehicleDesc;
    vehicle->mType = VEHICLE_FLAMETANK;
    vehicle->mImage = gImageLibrary->reference("data/interface/vehicleFlameTank.png");
    vehicle->mSize = vehicle->mImage->mSize;
    vehicle->mCenter = pos + vehicle->mSize / 2;
    pos[0] += vehicle->mSize[0] + border;
    mVehicles.push_back(vehicle);

    vehicle = new VehicleDesc;
    vehicle->mType = VEHICLE_TREBUCHET;
    vehicle->mImage = gImageLibrary->reference("data/interface/vehicleTrebuchet.png");
    vehicle->mSize = vehicle->mImage->mSize;
    vehicle->mCenter = pos + vehicle->mSize / 2;
    pos[0] += vehicle->mSize[0] + border;
    mVehicles.push_back(vehicle);
}

VehicleSelectView::~VehicleSelectView()
{
    gGame->unregisterTouchable(this);
    gGame->unregisterDrawable(this);
    gGame->unregisterUpdateable(this);
    
    gImageLibrary->unreference(mBackground);
    
    for(VehicleDescVector::iterator i=mVehicles.begin(); i !=mVehicles.end(); ++i)
    {
        gImageLibrary->unreference((*i)->mImage);
        delete *i;
    }
}

void VehicleSelectView::init()
{
    gGame->registerTouchable(this);
    gGame->registerDrawable(this);
    gGame->registerUpdateable(this);
}

void VehicleSelectView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    drawImage(mBackground, trans, 1);
    
    for(VehicleDescVector::iterator i=mVehicles.begin(); i != mVehicles.end(); ++i)
    {
        VehicleDesc* vehicleDesc = *i;
        trans = Transform2::createTranslation(vehicleDesc->mCenter - vehicleDesc->mSize/2) *
                Transform2::createScale(vehicleDesc->mSize);
        drawImage(vehicleDesc->mImage, trans, 1);
    }
}

bool VehicleSelectView::touch(uint32_t count, Touch* touches)
{
    // No multitouch, and only on release.
    if(count != 1)
        return false;

    if(touches[0].mPhase != Touch::PhaseEnd)
        return false;
    
    Vector2 pos = touches[0].mLocation;
    
    // Figure out if you touched anything.
    for(VehicleDescVector::iterator i=mVehicles.begin(); i != mVehicles.end(); ++i)
    {
        VehicleDesc* vehicleDesc = *i;
        Vector2 ul = vehicleDesc->mCenter - vehicleDesc->mSize/2;
        Vector2 lr = vehicleDesc->mCenter + vehicleDesc->mSize/2;
        
        // Is this touch inside of this vehicledesc?
        if(pos.x < ul.x || pos.y < ul.y || pos.x > lr.x || pos.y > lr.y) 
            continue;
            
        dprintf("Selected %d", vehicleDesc->mType);
        selectVehicle(vehicleDesc->mType);
        return true;
    }
    return false;
}

void VehicleSelectView::selectVehicle(uint32_t type)
{
    new World("fallenCity", type);

    // Delete ourselves.
    gGame->queueDeleteable(this);
}

void VehicleSelectView::update()
{}

}

