
#include "deathCardView.h"
#include "imageLibrary.h"
#include "drawImage.h"

namespace pammo
{

DeathCardView::DeathCardView()
    : View()
{
    // Load background.
    mBackground = gImageLibrary->reference("data/interface/deathCards/00.png");
    mAlpha = 0;
}

DeathCardView::~DeathCardView()
{
    gImageLibrary->unreference(mBackground);
}
    
uint32_t DeathCardView::getDrawPriority() const
{
    return kDeathCardPriority;
}

uint32_t DeathCardView::getUpdatePriority() const
{
    return kDeathCardPriority;
}

void DeathCardView::update()
{
    if(mAlpha > 1.0f)
        mAlpha = 1.0f;
    else
        mAlpha+=0.5f;
}

void DeathCardView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    trans *= Transform2::createTranslation(Vector2(0.10f, 0.065f));
    drawImage(mBackground, trans, mAlpha);
}


} // namespace pammo

