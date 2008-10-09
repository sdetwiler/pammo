#include "imageEntity.h"

namespace pammo
{

ImageEntity::ImageEntity()
{
    mAlpha = 1.0f;
    mRotation = 0;
	mCenter.x = 0;
	mCenter.y = 0;
    mSize.x = 0;
    mSize.y = 0;
    
    mImage = 0;
    mDirty = true;
};

ImageEntity::ImageEntity(Image* image)
{
    mAlpha = 1.0f;
	mCenter.x = 0;
	mCenter.y = 0;
    mSize.x = 0;
    mSize.y = 0;
    
    mImage = 0;
    mDirty = true;
    
    setImage(image);
}

ImageEntity::~ImageEntity()
{}

void ImageEntity::makeDirty()
{
    mDirty = true;
}

void ImageEntity::setImage(Image* image)
{
    assert(image);
    mImage = image;
    mSize = mImage->mSize;
    mDirty = true;
}

void ImageEntity::setImageWithoutSize(Image* image)
{
    assert(image);
    mImage = image;
    mDirty = true;
}

void ImageEntity::setImageAndInit(Image* image)
{
    assert(image);
    mAlpha = 1.0f;
	mCenter.x = 0;
	mCenter.y = 0;
    mSize.x = 0;
    mSize.y = 0;
    
    mImage = 0;
    mDirty = true;
    setImage(image);
}


Image* ImageEntity::getImage()
{
    return mImage;
}

void ImageEntity::draw()
{
#ifdef PROFILE
    static int lastTexture = 0;
    static uint32_t tick=0;
    static uint32_t total=0;
#endif
    if(mDirty)
    {
        Transform2 t = Transform2::createRotation(mRotation);
        
        mVertices[0] = Vector2(-mSize.x/2, -mSize.y/2) * t + mCenter;
        mVertices[1] = Vector2(mSize.x/2, -mSize.y/2) * t + mCenter;
        mVertices[2] = Vector2(-mSize.x/2, mSize.y/2) * t + mCenter;
        mVertices[3] = Vector2(mSize.x/2, mSize.y/2) * t + mCenter;
        
        mDirty = false;
    }
    
    glLoadIdentity();

    glVertexPointer(2, GL_FLOAT, 0, (float*)mVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)mImage->mTexCoords);

    glColor4f(1.0f, 1.0f, 1.0f, mAlpha);

    //if(lastTexture != mImage->mTexture)
    {
        glBindTexture(GL_TEXTURE_2D, mImage->mTexture);

//        lastTexture = mImage->mTexture;
    }

#ifdef PROFILE
    uint64_t start = getTime();
#endif

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef PROFILE
    uint64_t end = getTime();
#endif
    
	glColor4f(1, 1, 1, 1);

#ifdef PROFILE
    uint32_t delta = (uint32_t)(end-start);
    total+=delta;
    ++tick;
    if(!(tick%200))
    {
        dprintf("glDrawArrays took %u\t(%.2f ave)", 
            delta,
            (float)(total)/(float)tick
            );
    }
#endif
}


} // namespace pammo