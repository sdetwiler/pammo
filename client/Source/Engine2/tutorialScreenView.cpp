
#include "tutorialScreenView.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "mainScreenView.h"

namespace pammo
{

TutorialScreenView::TutorialScreenView()
    : View()
{
//    gTutorialScreenView = this;
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/tutorialScreenBackground.png");
}

TutorialScreenView::~TutorialScreenView()
{
    gImageLibrary->unreference(mBackground);
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
    
    destroy();
    new MainScreenView;

    return true;
}

} // namespace pammo

