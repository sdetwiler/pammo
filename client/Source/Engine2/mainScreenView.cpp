
#include "mainScreenView.h"
#include "world.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "player.h"

namespace pammo
{

MainScreenView* gMainScreenView = NULL;

MainScreenView::MainScreenView()
    : View()
{
    gMainScreenView = this;
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/mainScreenBackground.png");
}

MainScreenView::~MainScreenView()
{
    gImageLibrary->unreference(mBackground);
}
    
uint32_t MainScreenView::getTouchPriority() const
{
    return kScreenPriority;
}

uint32_t MainScreenView::getDrawPriority() const
{
    return kScreenPriority;
}

uint32_t MainScreenView::getUpdatePriority() const
{
    return kScreenPriority;
}

void MainScreenView::update()
{
    if(!gWorld)
    {
        new World;
        gWorld->disable();
    }
}

void MainScreenView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    drawImage(mBackground, trans, 1);
}

bool MainScreenView::touch(uint32_t count, Touch* touches)
{
    // No multitouch, and only on release.
    if(count != 1)
        return false;

    if(touches[0].mPhase != Touch::PhaseEnd)
        return false;
    
    Vector2 pos = touches[0].mLocation;

    disableAll();
    //gWorld->getPlayer()->setCenter(gWorld->getPlayer()->getSpawnPoint());
    gWorld->enable();

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

//void MainScreenView::selectVehicle(uint32_t type)
//{
//    new World(mMapName.c_str(), type);
//
//    // Delete ourselves.
//    gGame->queueDeleteable(this);
//}

} // namespace pammo

