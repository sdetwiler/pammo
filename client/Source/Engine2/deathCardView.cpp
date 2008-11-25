
#include "deathCardView.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "player.h"
#include "world.h"

#define MAX_SCORE 50000.0f

namespace pammo
{

DeathCardView::DeathCardView()
    : View()
{
    mTouched = false;
    
    // Get player.
    Player* player = gWorld->getPlayer();

    int card = ((float)player->mScore/MAX_SCORE) * INTERFACE_DEATH_CARD_COUNT;
    
    if(card>=INTERFACE_DEATH_CARD_COUNT)
        card = INTERFACE_DEATH_CARD_COUNT-1;

    // Load background.
    mBackground = gImageLibrary->getImage(INTERFACE_DEATH_CARD_00 + card);
    mAlpha = 0;
}

DeathCardView::~DeathCardView()
{
    gImageLibrary->purgeImage(mBackground);
}
    
uint32_t DeathCardView::getDrawPriority() const
{
    return kDeathCardPriority;
}

uint32_t DeathCardView::getUpdatePriority() const
{
    return kDeathCardPriority;
}

uint32_t DeathCardView::getTouchPriority() const
{
    return kDeathCardPriority;
}

bool DeathCardView::isTouched() const
{
    return mTouched;
}
    
void DeathCardView::update()
{
    if(mAlpha > 1.0f)
        mAlpha = 1.0f;
    else
        mAlpha+=0.05f;
}

void DeathCardView::draw()
{
    Transform2 trans = Transform2::createScale(mBackground->mSize);
    trans *= Transform2::createTranslation(Vector2(0.08f, 0.020f));
    drawImage(mBackground, trans, mAlpha);
}

bool DeathCardView::touch(uint32_t count, Touch* touches)
{
    mTouched = true;
    return true;
}
    
    
} // namespace pammo

