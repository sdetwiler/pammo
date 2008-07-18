#ifndef __IMAGEENTITY_H__
#define __IMAGEENTITY_H__

#include "pammo.h"

namespace pammo
{

class Image;

class ImageEntity
{
public:
    ImageEntity();
    ImageEntity(Image* image);
    ~ImageEntity();

    void draw();

    void setImage(Image* image);
    Image* getImage();

    void makeDirty();
		
    float mAlpha;
    Vector2 mCenter;
    Vector2 mSize;
    float mRotation;
    
protected:
    Image* mImage;
    bool mDirty;
    Vector2 mVertices[4];

private:

};

} // namespace pammo

#endif // __IMAGEENTITY_H__