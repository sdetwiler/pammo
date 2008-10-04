
#include "deathCardView.h"
#include "imageLibrary.h"
#include "drawImage.h"
#include "player.h"
#include "world.h"

#define NUM_CARDS 11
#define MAX_SCORE 3000.0f

namespace pammo
{

DeathCardView::DeathCardView()
    : View()
{
    // Get player.
    Player* player = gWorld->getPlayer();


    int card = ((float)player->mScore/MAX_SCORE) * NUM_CARDS;
    
    if(card>NUM_CARDS)
        card = NUM_CARDS;

    // Load background.
    char filename[256];
    sprintf(filename, "data/interface/deathCards/0%d.png", card);
    mBackground = gImageLibrary->reference(filename);
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
    trans *= Transform2::createTranslation(Vector2(0.08f, 0.020f));
    drawImage(mBackground, trans, mAlpha);
}


} // namespace pammo

