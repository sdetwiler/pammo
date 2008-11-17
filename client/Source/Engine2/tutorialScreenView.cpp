
#include "tutorialScreenView.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "mainScreenView.h"

namespace pammo
{

TutorialScreenView::TutorialScreenView()
    : View()
{
    mStage = 0;
//    gTutorialScreenView = this;
    // Load background.
    mBackground = gImageLibrary->getImage(INTERFACE_BACKGROUND_TUTORIAL0);
}

TutorialScreenView::~TutorialScreenView()
{
    gImageLibrary->purgeImage(mBackground);
}
    
uint32_t TutorialScreenView::getTouchPriority() const
{
    return kTutorialScreenPriority;
}

uint32_t TutorialScreenView::getDrawPriority() const
{
    return kTutorialScreenPriority;
}

uint32_t TutorialScreenView::getUpdatePriority() const
{
    return kTutorialScreenPriority;
}

void TutorialScreenView::update()
{
}

void TutorialScreenView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    drawImage(mBackground, trans, 1);
}

bool TutorialScreenView::touch(uint32_t count, Touch* touches)
{
    // No multitouch, and only on release.
    if(count != 1)
        return false;

    if(touches[0].mPhase != Touch::PhaseEnd)
        return false;
    
    ++mStage;
    switch(mStage)
    {
        case 1:
            gImageLibrary->purgeImage(mBackground);
            mBackground = gImageLibrary->getImage(INTERFACE_BACKGROUND_TUTORIAL1);
            break;
        case 2:
            gImageLibrary->purgeImage(mBackground);
            mBackground = gImageLibrary->getImage(INTERFACE_BACKGROUND_TUTORIAL2);
            break;
        default:
            destroy();
            new MainScreenView;
    }
    return true;
}

} // namespace pammo

