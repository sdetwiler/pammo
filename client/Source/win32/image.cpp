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

    int format;
    if(surface->format->BitsPerPixel == 24)
    {
        img->mBytesPerPixel = 3;
    }
    else if(surface->format->BitsPerPixel = 32)
    {
        img->mBytesPerPixel = 4;
    }

    img->mPixels = new uint8_t[img->mBytesPerPixel * img->mSize.x * img->mSize.y];
    memcpy(img->mPixels, surface->pixels, img->mBytesPerPixel * img->mSize.x * img->mSize.y);

    SDL_FreeSurface(surface);
}

}
