#include "image.h"
#include <map>
#include <string>
#include <SDL_image.h>

Vector2 getFrameSize()
{
    return Vector2(320, 480);
}

struct ImageRef
{
    Image* mImage;
    uint32_t mRefCount;
};

typedef std::map< std::string, ImageRef*> StringImageRefMap;
StringImageRefMap* gImages=NULL;

Image* openImage(char const* path)
{
    if(gImages == NULL)
        gImages = new StringImageRefMap;
    std::string str = path;
    StringImageRefMap::iterator i = gImages->find(str);
    if(i!= gImages->end())
    {
        ++(i->second->mRefCount);
        return i->second->mImage;
    }

    Image* image = new Image;
    SDL_Surface* surface = IMG_Load(path);
    if(!surface)
    {
        dprintf("Failed to load %s\n", path);
        delete image;
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

    int format;
    if(surface->format->BitsPerPixel == 24)
        format = GL_RGB;
    else if(surface->format->BitsPerPixel = 32)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glDisable(GL_TEXTURE_2D);

    SDL_FreeSurface(surface);

    str = path;
    ImageRef* ref = new ImageRef;
    ref->mImage = image;
    ref->mRefCount = 1;
    gImages->insert(std::pair<std::string, ImageRef*>(str, ref));

    return image;
}

void closeImage(Image* image)
{
    // SCD TOTAL CRAP.
    for(StringImageRefMap::iterator i = gImages->begin(); i!=gImages->end(); ++i)
    {
        if(i->second->mImage == image)
        {
            --(i->second->mRefCount);
            if(!i->second->mRefCount)
            {
                glDeleteTextures(1, &i->second->mImage->mTexture);
                delete i->second;
                gImages->erase(i);
                delete image;
                return;
            }
        }
    }
}