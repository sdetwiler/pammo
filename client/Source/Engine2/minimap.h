#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "pammo.h"

#include "view.h"

namespace pammo
{

class ImageEntity;
const uint32_t kMaxMarkers = 200;

enum MarkerType
{
    kMinimapPlayerMarker = 1,
    kMinimapPowerupMarker,
    kMinimapEnemyMarker,
};

class Minimap : public View
{
    public:
        Minimap();
        virtual ~Minimap();
        
        void setMapSize(Vector2 mapBounds);
        void mark(Vector2 pos, MarkerType marker);
        
        virtual uint32_t getDrawPriority() const;
        virtual void draw();
    
    private:
        Vector2 mMapSize;
        uint32_t mMarkerCount;
        Vector2 mLocations[kMaxMarkers];
        uint8_t mTypes[kMaxMarkers];
        ImageEntity* mBackground;
};

}

#endif