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
    ImageEntity(Image* image);
    virtual ~ImageEntity();

    virtual void draw();

protected:
    Image* mImage;

private:

};

} // namespace pammo

#endif // __IMAGEENTITY_H__