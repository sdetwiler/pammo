#include "imageEntity.h"

namespace pammo
{

const uint32_t kInvalidTextureCache = 0xFFFFFFFF;
uint32_t ImageEntity::mTextureCache = kInvalidTextureCache;

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

void ImageEntity::resetTextureCache()
{
    mTextureCache = kInvalidTextureCache;
}

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
    static uint32_t tick=0;
    static uint32_t build=0;
    static uint32_t setup=0;
    static uint32_t bind=0;
    static uint32_t draw=0;
    uint64_t start;
    uint64_t end;
#endif

#ifdef PROFILE
    start = getTime();
#endif

    if(mDirty)
    {
        float cosa = cos(mRotation);
        float sina = sin(mRotation);
        
        float sx = mSize.x;
        float sy = mSize.y;
        
        float a = sx * cosa * 0.5;
        float b = -sy * sina * 0.5;
        float c = sx * sina * 0.5;
        float d = sy * cosa * 0.5;
        
        float x = mCenter.x;
        float y = mCenter.y;
        
        mVertices[0] = -a + -b + x;
        mVertices[1] = -c + -d + y;
        
        mVertices[2] =  a + -b + x;
        mVertices[3] =  c + -d + y;
        
        mVertices[4] = -a +  b + x;
        mVertices[5] = -c +  d + y;
        
        mVertices[6] =  a +  b + x;
        mVertices[7] =  c +  d + y;
        
        mDirty = false;
    }

#ifdef PROFILE
    end = getTime();
    build += (uint32_t)(end - start);
    start = end;
#endif

    glVertexPointer(2, GL_FLOAT, 0, (float*)mVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, (float*)mImage->mTexCoords);
    glColor4f(1.0f, 1.0f, 1.0f, mAlpha);

#ifdef PROFILE
    end = getTime();
    setup += (uint32_t)(end - start);
    start = end;
#endif

    if(mTextureCache != mImage->mTexture)
    {
        glBindTexture(GL_TEXTURE_2D, mImage->mTexture);
        mTextureCache = mImage->mTexture;
    }

#ifdef PROFILE
    end = getTime();
    bind += (uint32_t)(end - start);
    start = end;
#endif

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

#ifdef PROFILE
    end = getTime();
    draw += (uint32_t)(end - start);
    start = end;
#endif

#ifdef PROFILE
    ++tick;
    static float count = 3200;
    if(tick >= count)
    {
        tick = 0;
        dprintf("Draw Report:\n\tBuild: %f\n\tSetup: %f\n\tBind: %f\n\tDraw: %f",
            build/count, setup/count, bind/count, draw/count);
        
        build = setup = bind = draw = 0;
    }
#endif
}


} // namespace pammo