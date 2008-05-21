#include "imageEntity.h"

namespace pammo
{

ImageEntity::ImageEntity(Image* image)
    : Entity()
{
    mImage = image;
	setSize(mImage->mSize);
}

ImageEntity::~ImageEntity()
{}

void ImageEntity::draw()
{
    drawImage(mImage, getTransform());
}


} // namespace pammo