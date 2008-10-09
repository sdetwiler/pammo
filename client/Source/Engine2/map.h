#ifndef __MAP_H__
#define __MAP_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{
    
class Camera;
struct Image;
class ImageEntity;
    
class Map : public View
{
public:
    Map();
    virtual ~Map();
    
    void loadBackdrop(char const* backdropName);
    
    //void addProp(ImageEntity* entity);
    
    virtual uint32_t getDrawPriority() const;
    void draw();
    void lowMemory();
    
    void setSize(Vector2 const& size);
    Vector2 getSize() const;

protected:
    //void addSubProp(Image* image, Vector2 ul, Vector2 br, uint16_t x, uint16_t y);
    
    struct Prop
    {
        Image* mImage;
        uint32_t mId;
        Vector2 mVertecies[4];
        Vector2 mTexCoords[4];
        Vector2 mPreviewTexCoords[4];
        //Prop* mNext;
    };
    
    Image* mPreview;
    
    uint32_t mSizeX, mSizeY;
    uint32_t mBucketSizeX, mBucketSizeY;
    uint32_t mBucketCountX, mBucketCountY;
    Prop** mBuckets;
    
    //typedef vector< ImageEntity* > ImageEntityVector;
    //ImageEntityVector mEntities;
};

}
#endif