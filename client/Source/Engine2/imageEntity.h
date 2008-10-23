#ifndef __IMAGEENTITY_H__
#define __IMAGEENTITY_H__

#include "pammo.h"

namespace pammo
{

struct Image;

class ImageEntity
{
public:
    ImageEntity();
    ImageEntity(Image* image);
    ~ImageEntity();

    void draw();

    void setImageAndInit(Image* image);
    void setImage(Image* image);
    void setImageWithoutSize(Image* image);
    Image* getImage();
    
    static void resetTextureCache();

    void makeDirty();
		
    float mAlpha;
    Vector2 mCenter;
    Vector2 mSize;
    float mRotation;
    
protected:
    Image* mImage;
    bool mDirty;
    float mVertices[8];
    static uint32_t mTextureCache;
};

} // namespace pammo

#endif // __IMAGEENTITY_H__