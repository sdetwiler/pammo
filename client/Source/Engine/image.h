#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "pammo.h"
#include "types_platform.h"

namespace pammo
{

struct Image
{
    Vector2 mSize;
    uint32_t mTexture;
};

// per platform.
Image* openImage(char const* filename);
void closeImage(Image* image);
Vector2 getFrameSize();


void initImage();
void drawImage(Image* image, Transform2 const& transform, float alpha=1.0f);

}

#endif // __IMAGE_H__
