#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

class ImageEntity;
const uint32_t kMinimapBucketCount = 15;
const uint32_t kMaxEnemies = 200;

class Minimap : public View
{
    public:
        Minimap();
        virtual ~Minimap();
        
        void setMapSize(Vector2 mapBounds);
        void markEnemy(Vector2 pos);
        
        virtual uint32_t getDrawPriority() const;
        virtual void draw();
    
    private:
        //Vector2 mBucketSize;
        //bool mBuckets[kMinimapBucketCount][kMinimapBucketCount];
        Vector2 mMapSize;
        uint32_t mEnemyCount;
        Vector2 mLocations[kMaxEnemies];
        ImageEntity* mBackground;
};

}

#endif