#include "scoreMeter.h"

#include "drawImage.h"
#include "imageLibrary.h"

namespace pammo
{

ScoreMeter::ScoreMeter()
{
    mScore = 0;
    for(int i=0; i<10; ++i)
    {
        char filename[256];
        //snprintf(filename, 255, "data/fonts/caslonAntique/%d.png", i);
        mImages[i] = gImageLibrary->getImage(FONT_CASLONANTIQUE_0 + i);
    }
}

ScoreMeter::~ScoreMeter()
{

}

uint32_t ScoreMeter::getDrawPriority() const
{
    return kScoreMeterPriority;
}

void ScoreMeter::setScore(uint32_t score)
{
    mScore = score;
}

void ScoreMeter::draw()
{
    int baseX = (480/2) - (4*16);
    int baseY = 320 - 16;

    uint32_t score = mScore;
    int x = baseX + (6 * 16);

    for(int i=0; i<6; ++i)
    {
        uint32_t s = score%10;
        Image* image = mImages[s];

        Transform2 trans = Transform2::createTranslation(Vector2(x, baseY));
        trans*= Transform2::createScale(image->mSize);
        
        drawImage(image, trans, 1.0f);
        x-=16;

        score/=10;
    }
}

} // namespace pammo