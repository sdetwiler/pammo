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