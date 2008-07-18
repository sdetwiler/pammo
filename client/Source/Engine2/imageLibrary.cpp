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


