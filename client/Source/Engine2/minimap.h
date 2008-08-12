#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

const uint32_t kMapBucketCount = 15;

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
        Vector2 mBucketSize;
        bool mBuckets[kMapBucketCount][kMapBucketCount];
};

}

#endif