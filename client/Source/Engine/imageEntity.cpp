#include "imageEntity.h"

namespace pammo
{

ImageEntity::ImageEntity()
    : Entity()
{
    mImage = 0;
    mAlpha= 1.0f;
};

ImageEntity::ImageEntity(Image* image)
    : Entity()
{
    setImage(image);
}

ImageEntity::~ImageEntity()
{}

void ImageEntity::setImage(Image* image)
{
    mImage = image;
    setSize(mImage->mSize);
    mAlpha = 1.0;
}

void ImageEntity::draw()
{
    drawImage(mImage, getTransform(), mAlpha);
}


} // namespace pammo