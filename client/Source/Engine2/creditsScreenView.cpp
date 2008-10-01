
#include "creditsScreenView.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "mainScreenView.h"

namespace pammo
{

CreditsScreenView::CreditsScreenView()
    : View()
{
//    gCreditsScreenView = this;
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/creditsScreenBackground.png");
}

CreditsScreenView::~CreditsScreenView()
{
    gImageLibrary->unreference(mBackground);
}
    
uint32_t CreditsScreenView::getTouchPriority() const
{
    return kCreditsScreenPriority;
}

uint32_t CreditsScreenView::getDrawPriority() const
{
    return kCreditsScreenPriority;
}

uint32_t CreditsScreenView::getUpdatePriority() const
{
    return kCreditsScreenPriority;
}

void CreditsScreenView::update()
{
}

void CreditsScreenView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    drawImage(mBackground, trans, 1);
}

bool CreditsScreenView::touch(uint32_t count, Touch* touches)
{
    // No multitouch, and only on release.
    if(count != 1)
        return false;

    if(touches[0].mPhase != Touch::PhaseEnd)
        return false;
    
    destroy();
    new MainScreenView;

    // Figure out if you touched anything.
    //for(VehicleDescVector::iterator i=mVehicles.begin(); i != mVehicles.end(); ++i)
    //{
    //    VehicleDesc* vehicleDesc = *i;
    //    Vector2 ul = vehicleDesc->mCenter - vehicleDesc->mSize/2;
    //    Vector2 lr = vehicleDesc->mCenter + vehicleDesc->mSize/2;
    //    
    //    // Is this touch inside of this vehicledesc?
    //    if(pos.x < ul.x || pos.y < ul.y || pos.x > lr.x || pos.y > lr.y) 
    //        continue;
    //        
    //    dprintf("Selected %d", vehicleDesc->mType);
    //    selectVehicle(vehicleDesc->mType);
    //    return true;
    //}
    return true;
}

} // namespace pammo

