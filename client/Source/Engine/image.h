#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "pammo.h"
#include "types_platform.h"

struct Image
{
    Vector2 mSize;
    uint32_t mTexture;
};

// per platform.
Image* openImage(char const* filename);
void closeImage(Image* image);
Vector2 getFrameSize();


void drawImage(Image* image, Transform2 const& transform);


#endif // __IMAGE_H__
