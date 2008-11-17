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
        mImages[i] = gImageLibrary->getImage(FONT_CASLONANTIQUE_0 + i);
    }
    
    mStep = 0.015f;
    mGlowImageScale = 0.95f;
    mGlowImage = gImageLibrary->getImage(INTERFACE_SCOREGLOW);
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
 //   if(score != mScore)
 //       mGlowImageScale = .001;

    mScore = score;
}

void ScoreMeter::draw()
{
    int baseX = (480/2) - (4*16);
    int baseY = 320 - 16;
    Transform2 trans = Transform2::createTranslation(Vector2(baseX+16, baseY));
    trans*= Transform2::createScale(mGlowImage->mSize);
    drawImage(mGlowImage, trans, 1);

    if(mGlowImageScale < 0.65 || mGlowImageScale > 0.95)
    {
        mStep*=-1;
    }
    
    mGlowImageScale+=mStep;

//    dprintf("%.2f", mGlowImageScale);
    uint32_t score = mScore;
    int x = baseX + (6 * 16);

    for(int i=0; i<6; ++i)
    {
        uint32_t s = score%10;
        Image* image = mImages[s];

        trans = Transform2::createTranslation(Vector2(x, baseY));
        trans*= Transform2::createScale(image->mSize);
        
        drawImage(image, trans, mGlowImageScale);
        x-=16;

        score/=10;
    }
    
//    //if(mGlowImageScale < 0.95)
//    {
//        Transform2 trans = Transform2::createTranslation(Vector2(baseX+16, baseY));
//        trans*= Transform2::createScale(mGlowImage->mSize);
//        drawImage(mGlowImage, trans, 0.95f - mGlowImageScale);
//        mGlowImageScale+=step;
//    }
    
}

} // namespace pammo