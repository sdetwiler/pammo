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
    
    if(mTargetPercent < 0) mTargetPercent = 0;
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

// RGB and HSV conversion.
// http://www.cs.rit.edu/~ncs/color/t_convert.html
void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float min, max, delta;
    
    if(r < g && r < b) min = r;
    else if(g < r && g < b) min = g;
    else min = b;
    
    if(r > g && r > b) max = r;
    else if(g > r && g > b) max = g;
    else max = b;

	//min = MIN( r, g, b );
	//max = MAX( r, g, b );
	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

    *h *= M_PI/3;
	//*h *= 60;				// degrees
	//if( *h < 0 )
	//	*h += 360;

}

void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;

	if( s == 0 )
    {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= M_PI/3;		// sector 0 to 5
	//h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i )
    {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

void BarMeter::draw()
{
    float priR, priG, priB;
    float secR, secG, secB;
    bool sec = false;
    
    float percent = fmod(mCurrentPercent, 1);
    float rotates = floor(mCurrentPercent);
    
    if(rotates == 0)
    {
        priR = mR;
        priG = mG;
        priB = mB;
    }
    else
    {
        sec = true;
    
        float h, s, v;
        RGBtoHSV(mR, mG, mB, &h, &s, &v);
        
        float priH, secH;
        priH = fmod(h + mGrowDirection*M_PI/5*rotates, 2*M_PI);
        secH = fmod(h + mGrowDirection*M_PI/5*(rotates - 1), 2*M_PI);
        
        HSVtoRGB(&priR, &priG, &priB, priH, s, v);
        HSVtoRGB(&secR, &secG, &secB, secH, s, v);
    }
    
    float priVertLeft, priVertRight;
    float priTexLeft, priTexRight;
    float secVertLeft, secVertRight;
    float secTexLeft, secTexRight;
    
    const float texLength = 150;
    const float imageLength = 256;
    
    if(mGrowDirection > 0)
    {
        priVertLeft = mBaseLocation.x;
        priVertRight = mBaseLocation.x + percent*texLength;
        
        priTexLeft = 0;
        priTexRight = percent*texLength/imageLength;
        
        secVertLeft = priVertRight;
        secVertRight = mBaseLocation.x + texLength;
        
        secTexLeft = priTexRight;
        secTexRight = texLength/imageLength;
    }
    else
    {
        priVertLeft = mBaseLocation.x + (1-percent)*texLength;
        priVertRight = mBaseLocation.x + texLength;
        
        priTexLeft = (1-percent)*texLength/imageLength;
        priTexRight = texLength/imageLength;
        
        secVertLeft = mBaseLocation.x;
        secVertRight = mBaseLocation.x + (1-percent)*texLength;
        
        secTexLeft = 0;
        secTexRight = priTexLeft;
    }

    glBindTexture(GL_TEXTURE_2D, mMeterBar->mTexture);
    
    if(sec)
    {
        GLfloat secVert[] = {
            secVertLeft, mBaseLocation.y,
            secVertRight, mBaseLocation.y,
            secVertLeft, mBaseLocation.y + mMeterBar->mSize.y,
            secVertRight, mBaseLocation.y + mMeterBar->mSize.y,
        };

        GLfloat secTex[] = {
            secTexLeft, 0.0,
            secTexRight, 0.0,
            secTexLeft, 1.0,
            secTexRight, 1.0,
        };
        glColor4f(secR, secG, secB, 1);
        glVertexPointer(2, GL_FLOAT, 0, secVert);
        glTexCoordPointer(2, GL_FLOAT, 0, secTex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    GLfloat priVert[] = {
        priVertLeft, mBaseLocation.y,
        priVertRight, mBaseLocation.y,
        priVertLeft, mBaseLocation.y + mMeterBar->mSize.y,
        priVertRight, mBaseLocation.y + mMeterBar->mSize.y,
    };

    GLfloat priTex[] = {
        priTexLeft, 0.0,
        priTexRight, 0.0,
        priTexLeft, 1.0,
        priTexRight, 1.0,
    };
    
    glColor4f(priR, priG, priB, 1);
    glVertexPointer(2, GL_FLOAT, 0, priVert);
    glTexCoordPointer(2, GL_FLOAT, 0, priTex);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glLoadIdentity();
    ImageEntity::resetTextureCache();
}

} // namespace pammo