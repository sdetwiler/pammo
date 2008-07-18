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
    
    void setNumMaterials(uint16_t numMaterials, uint16_t sizeMaterial = 128);
    void setMaterial(uint16_t index, char const* materialPath);
    
    void setNumTiles(uint16_t sizeX, uint16_t sizeY);
    void setTile(uint16_t x, uint16_t y, uint16_t index);
    
    void addProp(ImageEntity* entity);
    
    virtual uint32_t getDrawPriority() const;
    void draw();
    
    uint16_t getSizeX() const;
    uint16_t getSizeY() const;
    uint16_t getSizeMaterial() const;

protected:
    void addSubProp(Image* image, Vector2 ul, Vector2 br, uint16_t x, uint16_t y);
    
    void drawTiles(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);
    void drawProps(uint16_t startX, uint16_t startY, uint16_t endX, uint16_t endY);
    void drawEntities();

    uint16_t mNumMaterials;
    Image** mMaterials;
    uint16_t mSizeMaterial;
    uint16_t mSizeX;
    uint16_t mSizeY;
    uint16_t* mTiles;
    
    struct Prop
    {
        Image* mImage;
        Vector2 mVertecies[4];
        Vector2 mTexCoords[4];
        Prop* mNext;
    };
    Prop** mProps;
    
    typedef vector< ImageEntity* > ImageEntityVector;
    ImageEntityVector mEntities;
};

}
#endif