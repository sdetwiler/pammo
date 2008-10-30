#include "barMeter.h"

#include "drawImage.h"
#include "imageLibrary.h"
#include "imageEntity.h"

namespace pammo
{

BarMeter::BarMeter(uint32_t priority)
{
    mPriority = priority;
    mTargetPercent = 1;
    mCurrentPercent = 1;
    mMeterBar = gImageLibrary->getImage(INTERFACE_BAR_METER);
    
    mGrowDirection = 1;
    mBaseLocation = Vector2(0, 0);
    
    mR = mG = mB = 0;
}

BarMeter::~BarMeter()
{

}

uint32_t BarMeter::getUpdatePriority() const
{
    return mPriority;
}

uint32_t BarMeter::getDrawPriority() const
{
    return mPriority;
}

void BarMeter::setTargetPercent(float percent)
{
    mTargetPercent = percent;
}

void BarMeter::setCurrentPercent(float percent)
{
    mCurrentPercent = mTargetPercent = percent;
}

void BarMeter::setGrowDirection(int growDirection)
{
    mGrowDirection = growDirection;
}

void BarMeter::setBaseLocation(Vector2 baseLocation)
{
    mBaseLocation = baseLocation;
}

void BarMeter::setColor(float r, float g, float b)
{
    mR = r;
    mG = g;
    mB = b;
}

void BarMeter::update()
{
    if(mCurrentPercent < mTargetPercent)
    {
        mCurrentPercent += 0.015;
        if(mCurrentPercent > mTargetPercent)
            mCurrentPercent = mTargetPercent;
    }
    else if(mCurrentPercent > mTargetPercent)
    {
        mCurrentPercent -= 0.015;
        if(mCurrentPercent < mTargetPercent)
            mCurrentPercent = mTargetPercent;
    }
}

void BarMeter::draw()
{
    glColor4f(mR, mG, mB, 1);
    
    float percent = mCurrentPercent;
    
    float leftVert, rightVert;
    float leftTex, rightTex;
    
    const float texLength = 172;
    const float imageLength = 256;
    
    if(mGrowDirection > 0)
    {
        leftVert = mBaseLocation.x;
        rightVert = mBaseLocation.x + percent*texLength;
        
        leftTex = 0;
        rightTex = percent*texLength/imageLength;
    }
    else
    {
        leftVert = mBaseLocation.x + (1-percent)*texLength;
        rightVert = mBaseLocation.x + texLength;
        
        leftTex = (1-percent)*texLength/imageLength;
        rightTex = texLength/imageLength;
    }

    GLfloat vert[] = {
        leftVert, mBaseLocation.y,
        rightVert, mBaseLocation.y,
        leftVert, mBaseLocation.y + mMeterBar->mSize.y,
        rightVert, mBaseLocation.y + mMeterBar->mSize.y,
    };

    GLfloat tex[] = {
        leftTex, 0.0,
        rightTex, 0.0,
        leftTex, 1.0,
        rightTex, 1.0,
    };

    glBindTexture(GL_TEXTURE_2D, mMeterBar->mTexture);
    glVertexPointer(2, GL_FLOAT, 0, vert);
    glTexCoordPointer(2, GL_FLOAT, 0, tex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glLoadIdentity();
    ImageEntity::resetTextureCache();
}

} // namespace pammo