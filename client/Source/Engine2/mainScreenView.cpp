
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

    // credits
    options[0].ul = Vector2(0.0f, 0.0f);
    options[0].lr = Vector2(160.0f, 320.0f);

    //  learn
    options[1].ul = Vector2(160.0f, 0.0f);
    options[1].lr = Vector2(320.0f, 320.0f);

    // play
    options[2].ul = Vector2(320.0f, 0.0f);
    options[2].lr = Vector2(480.0f, 320.0f);


    for(uint32_t i=0; i<3; ++i)
    {

        if(pos.x < options[i].ul.x || pos.y < options[i].ul.y || pos.x > options[i].lr.x || pos.y > options[i].lr.y) 
            continue;
    
        destroy();
    
        switch(i)
        {
        case 0:
            new CreditsScreenView;
            return true;
        case 1:
            new TutorialScreenView;
            return true;
        case 2:
            gWorld->enable();
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

