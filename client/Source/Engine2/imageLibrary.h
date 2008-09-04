/*
 *  imageLibrary.h
 *  pammo
 *
 *  Created by James Marr on 6/1/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __IMAGE_LIBRARY_H__
#define __IMAGE_LIBRARY_H__

#include "pammo.h"

namespace pammo
{
    
struct Image;
 
class ImageLibrary
{
public:
    ImageLibrary();
    ~ImageLibrary();
    
    Image* reference(char const* path);
    void reference(Image* image);
    void unreference(Image* image);

    
private:
    struct ImageRef
    {
        Image* mImage;
        uint32_t mRefCount;
    };
    
    typedef map< string, ImageRef*> StringImageRefMap;
    StringImageRefMap mImages;
};
    
extern ImageLibrary* gImageLibrary;
    
}

#endif