#ifndef __MAP_H__
#define __MAP_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{
    
class Camera;
struct Image;
class ImageEntity;

const uint32_t kMapBucketCount = 15;
    
class Map : public View
{
public:
    Map();
    virtual ~Map();
    
    void setBackdrop(RawImage* raw);
    void addProp(ImageEntity* entity);
    
    virtual uint32_t getDrawPriority() const;
    void draw();
    
    Vector2 getSize() const;

protected:
    void addSubProp(Image* image, Vector2 ul, Vector2 br, uint16_t x, uint16_t y);
    
    struct Prop
    {
        Image* mImage;
        Vector2 mVertecies[4];
        Vector2 mTexCoords[4];
        Prop* mNext;
    };
    
    Vector2 mSize;
    Vector2 mBucketSize;
    Prop* mBuckets[kMapBucketCount][kMapBucketCount];
    
    typedef vector< ImageEntity* > ImageEntityVector;
    ImageEntityVector mEntities;
};

}
#endif