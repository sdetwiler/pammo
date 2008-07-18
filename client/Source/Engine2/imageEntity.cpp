#include "imageEntity.h"
#include "image.h"

namespace pammo
{

// Sets up an array of values for the texture coordinates.
const GLfloat spriteTexcoords[] = {
0.0, 0.0,
1.0, 0.0,
0.0, 1.0,
1.0, 1.0,
};

ImageEntity::ImageEntity()
{
    mAlpha = 1.0f;
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
    mImage = image;
    mSize = mImage->mSize;
    mDirty = true;
}
    
Image* ImageEntity::getImage()
{
    return mImage;
}

void ImageEntity::draw()
{
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
    glTexCoordPointer(2, GL_FLOAT, 0, spriteTexcoords);

    glColor4f(1.0f, 1.0f, 1.0f, mAlpha);
    glBindTexture(GL_TEXTURE_2D, mImage->mTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
	glColor4f(1, 1, 1, 1);
}


} // namespace pammo