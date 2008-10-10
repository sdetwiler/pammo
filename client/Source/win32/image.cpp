#include "pammo.h"
#include <map>
#include <string>
#include <SDL_image.h>

namespace pammo
{

Vector2 getFrameSize()
{
   return Vector2(480, 320);
}

void openRawImage(char const* path, RawImage* img)
{
//    Image* image = new Image;
    SDL_Surface* surface = IMG_Load(path);
    if(!surface)
    {
        dprintf("Failed to load %s\n", path);
        assert(0);  
    }

    img->mSize.x = (float)surface->w;
    img->mSize.y = (float)surface->h;

    uint32_t x = surface->w;
    uint32_t y = surface->h;

    // Check if x is power of two. If not, bump it up.
    if((x & (x-1)) != 0)
    {
        x--;
        x = (x >> 1) | x;
        x = (x >> 2) | x;
        x = (x >> 4) | x;
        x = (x >> 8) | x;
        x = (x >> 16) | x;
        x++;
    }
    
    // Check if y is power of two. If not, bump it up.
    if((y & (y-1)) != 0)
    {
        y--;
        y = (y >> 1) | y;
        y = (y >> 2) | y;
        y = (y >> 4) | y;
        y = (y >> 8) | y;
        y = (y >> 16) | y;
        y++;
    }

    img->mPixelSize.x = x;
    img->mPixelSize.y = y;
    if(surface->format->BitsPerPixel == 24)
    {
        img->mBytesPerPixel = 3;
    }
    else if(surface->format->BitsPerPixel = 32)
    {
        img->mBytesPerPixel = 4;
    }

    uint32_t pixelBytes = img->mPixelSize.x * img->mPixelSize.y * img->mBytesPerPixel;
	img->mPixels = new uint8_t[pixelBytes];
    memset(img->mPixels, 0, pixelBytes);

    // Lower left corner.
    uint32_t srcWidth = (uint32_t)img->mSize.x;
    uint32_t srcHeight = (uint32_t)img->mSize.y;
    for(uint32_t h=0; h<srcHeight; ++h)
    {
        int32_t dstOffset = x * (h + y - srcHeight) * img->mBytesPerPixel;
        int32_t srcOffset = surface->pitch * h;

        memcpy(img->mPixels + dstOffset, 
            ((uint8_t*)surface->pixels) + srcOffset, 
            srcWidth*img->mBytesPerPixel);
    }

    SDL_FreeSurface(surface);
}

}
