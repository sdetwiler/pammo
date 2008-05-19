#include "image.h"

#include <SDL_image.h>

Vector2 getFrameSize()
{
    return Vector2(320, 480);
}

Image* openImage(char const* path)
{
    Image* image = new Image;
    SDL_Surface* surface = IMG_Load(path);
    if(!surface)
    {
        return NULL;
    }
    image->mSize.x = (float)surface->w;
    image->mSize.y = (float)surface->h;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &image->mTexture);
    glBindTexture(GL_TEXTURE_2D, image->mTexture);
     
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); // scale linearly when image smalled than texture

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glDisable(GL_TEXTURE_2D);

    SDL_FreeSurface(surface);

    return image;
}

void closeImage(Image const* image)
{
    glDeleteTextures(1, &image->mTexture);
}
