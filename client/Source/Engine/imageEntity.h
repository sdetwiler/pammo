#ifndef __IMAGEENTITY_H__
#define __IMAGEENTITY_H__

#include "entity.h"
#include "image.h"

namespace pammo
{

class ImageEntity : 
    public Entity
{
public:
    ImageEntity();
    ImageEntity(Image* image);
    virtual ~ImageEntity();

    virtual void draw();

    void setImage(Image* image);
    Image* getImage();
    float mAlpha;
protected:
    Image* mImage;

private:

};

} // namespace pammo

#endif // __IMAGEENTITY_H__