
#include "mainScreenView.h"
#include "TutorialScreenView.h"
#include "CreditsScreenView.h"

#include "world.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "player.h"

namespace pammo
{

//MainScreenView* gMainScreenView = NULL;

MainScreenView::MainScreenView()
    : View()
{
    //gMainScreenView = this;
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/mainScreenBackground.png");
}

MainScreenView::~MainScreenView()
{
    gImageLibrary->unreference(mBackground);
}
    
uint32_t MainScreenView::getTouchPriority() const
{
    return kMainScreenPriority;
}

uint32_t MainScreenView::getDrawPriority() const
{
    return kMainScreenPriority;
}

uint32_t MainScreenView::getUpdatePriority() const
{
    return kMainScreenPriority;
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



    struct Box
    {
        Vector2 ul;
        Vector2 lr;
    };


    Box options[3];

    // tutorial
    options[0].ul = Vector2(20.0f, 200.0f);
    options[0].lr = Vector2(120.0f, 305.0f);

    //  play
    options[1].ul = Vector2(190.0f, 205.0f);
    options[1].lr = Vector2(280.0f, 280.0f);

    // credits
    options[2].ul = Vector2(355.0f, 200.0f);
    options[2].lr = Vector2(455.0f, 305.0f);


    for(uint32_t i=0; i<3; ++i)
    {

        if(pos.x < options[i].ul.x || pos.y < options[i].ul.y || pos.x > options[i].lr.x || pos.y > options[i].lr.y) 
            continue;
    
        destroy();
    
        switch(i)
        {
        case 0:
            new TutorialScreenView;
            return true;
        case 1:
            gWorld->enable();
            return true;
        case 2:
            new CreditsScreenView;
            return true;
        }
    }
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

