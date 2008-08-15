/*
 *  imageLibrary.cpp
 *  pammo
 *
 *  Created by James Marr on 6/1/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "imageLibrary.h"

namespace pammo
{

Image* makeSubImage(RawImage* raw, Vector2 start, Vector2 size)
{
	Image* image = new Image();
    image->mSize = size;
    
    // Setup pixel store.
    uint32_t srcRow = raw->mSize.x*4;
    uint32_t dstRow = size.x*4;
    uint8_t* pixels = (uint8_t*)malloc(dstRow*size.y);
    for(uint32_t y=0; y < size.y; ++y)
    {
        uint32_t srcOffset = y*dstRow;
        uint32_t dstOffset = start.x*4 + (start.y+y)*srcRow;
        uint8_t* src = pixels + srcOffset;
        uint8_t* dst = raw->mPixels + dstOffset;
        memcpy(src, dst, dstRow);
    }
    
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &image->mTexture);
	glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    // Restore pixel store.
    free(pixels);
    
    return image;
}

Image* openImage(char const* path)
{
    RawImage raw;
    openRawImage(path, &raw);
    
	Image* image = new Image();
    image->mSize = raw.mSize;
	
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &image->mTexture);
	glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->mSize.x, image->mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw.mPixels);
	
	free(raw.mPixels);
	return image;
}
    
ImageLibrary* gImageLibrary = NULL;

ImageLibrary::ImageLibrary()
{}

ImageLibrary::~ImageLibrary()
{
    for(StringImageRefMap::iterator i=mImages.begin(); i != mImages.end(); ++i)
    {
        glDeleteTextures(1, &i->second->mImage->mTexture);
        delete i->second->mImage;
        delete i->second;
    }
}

Image* ImageLibrary::reference(char const* path)
{
    string str = path;
    StringImageRefMap::iterator i = mImages.find(str);
    if(i != mImages.end())
    {
        ++(i->second->mRefCount);
        return i->second->mImage;
    }
    
    str = path;
    ImageRef* ref = new ImageRef;
    ref->mImage = openImage(path);
    ref->mRefCount = 1;
    mImages.insert(pair<string, ImageRef*>(str, ref));
    
    return ref->mImage;
}

void ImageLibrary::unreference(Image* image)
{
    // SCD TOTAL CRAP.
    for(StringImageRefMap::iterator i = mImages.begin(); i!=mImages.end(); ++i)
    {
        if(i->second->mImage == image)
        {
            --(i->second->mRefCount);
            if(!i->second->mRefCount)
            {
                glDeleteTextures(1, &i->second->mImage->mTexture);
                delete i->second->mImage;
                delete i->second;
                mImages.erase(i);
                return;
            }
        }
    }
}

}


