#ifndef __FLIPBOOKLOADER_H__
#define __FLIPBOOKLOADER_H__

#include "pammo.h"
struct Image;

namespace pammo
{

bool loadFlipbook(uint32_t baseId, uint32_t count, Image** images);
//bool loadFlipbook(char const* path, Image** images, uint32_t maxImages, uint32_t* numImages);

}

#endif __FLIPBOOKLOADER_H__