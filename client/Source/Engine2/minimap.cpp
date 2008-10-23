#include "minimap.h"

#include "world.h"
#include "player.h"
#include "imageLibrary.h"
#include "imageEntity.h"

namespace pammo
{

Minimap::Minimap()
    : View()
{
    //memset(mBuckets, 0, sizeof(mBuckets));
    
    mBackground = new ImageEntity(gImageLibrary->getImage(INTERFACE_MINIMAP));
    mBackground->mCenter = Vector2(240, 32);
    mBackground->mSize = Vector2(64, 64);
    mMarkerCount = 0;
}

Minimap::~Minimap()
{
}

void Minimap::setMapSize(Vector2 mapBounds)
{
    mMapSize = mapBounds;
    //mBucketSize = mapBounds / kMinimapBucketCount;
}

void Minimap::mark(Vector2 pos, MarkerType marker)
{
    if(mMarkerCount >= kMaxMarkers) return;
    
    mLocations[mMarkerCount] = pos / mMapSize;
    
    const float ins = 0.45;
    Vector2 tmp = mLocations[mMarkerCount] - Vector2(0.5, 0.5);
    float mag = magnitudeSquarded(tmp);
    if(mag > ins*ins)
    {
        tmp /= Vector2(sqrt(mag));
        tmp *= ins;
        mLocations[mMarkerCount] = tmp + Vector2(0.5, 0.5);
    }
    
    mTypes[mMarkerCount] = marker;
    ++mMarkerCount;
}

uint32_t Minimap::getDrawPriority() const
{
    return kMinimapPriority;
}

void Minimap::draw()
{
    Vector2 mapSize(64, 64);
    Vector2 mapOffset(208, 0);
    
    mBackground->draw();

    // Draw minimap.
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    // Draw outline.
    #if 0
    Vector2 outline[4];
    outline[0] = mapOffset;
    outline[1] = mapOffset + Vector2(mapSize.x, 0);
    outline[2] = mapOffset + Vector2(mapSize.x, mapSize.y);
    outline[3] = mapOffset + Vector2(0, mapSize.y);
    glColor4f(.1, .15, .8, .3);
    glVertexPointer(2, GL_FLOAT, 0, (float*)outline);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    #endif
    
    uint32_t const num = 8;
    Vector2 points[num];
    for(uint32_t j=0; j < num; ++j)
        points[j] = Vector2(mapSize.x/20, 0) * Transform2::createRotation(M_PI/4*j);// + mapSize.x/40;
    glVertexPointer(2, GL_FLOAT, 0, (float*)points);
    
    for(uint32_t i=0; i < mMarkerCount; ++i)
    {
        switch(mTypes[i])
        {
            case kMinimapPlayerMarker:
                //glColor4f(.1, .8, .1, .4);
                glColor4f(.95, .6, .1, .9);
                break;
            case kMinimapPowerupMarker:
                //glColor4f(.1, .1, .8, .4);
                glColor4f(.0, .55, .85, .85);
                break;
            case kMinimapEnemyMarker:
                //glColor4f(.8, .15, .1, .4);
                glColor4f(.8, .0, .0, .75);
                break;
        }
    
        glLoadIdentity();
        glTranslatef(mapOffset.x + mLocations[i].x*mapSize.x, mapOffset.y + mLocations[i].y*mapSize.y, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, num);
    }
    mMarkerCount = 0;
    
    //mMarkerCount = 3;
    //mLocations[0] = Vector2(0 + 1./20, 0.5);
    //mLocations[1] = Vector2(1 - 1./20, 0.5);
    //mLocations[2] = Vector2(1 - 1./20, 0.5) * Transform2::createTranslation(Vector2(-0.5, -0.5)) * Transform2::createRotation(M_PI/2) * Transform2::createTranslation(Vector2(0.5, 0.5));
    
    glLoadIdentity();
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ImageEntity::resetTextureCache();
}

}