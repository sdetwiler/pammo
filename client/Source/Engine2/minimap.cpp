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
    memset(mBuckets, 0, sizeof(mBuckets));
    
    mLifebar = new ImageEntity(gImageLibrary->reference("data/interface/shield-map-lifeBar.png"));
    mLifebar->mCenter = Vector2(240, 36);
    mLifebar->mSize = Vector2(256, 64);
}

Minimap::~Minimap()
{
}

void Minimap::setMapSize(Vector2 mapBounds)
{
    mBucketSize = mapBounds / kMinimapBucketCount;
}

void Minimap::markEnemy(Vector2 pos)
{
    int32_t x = floor(pos.x / mBucketSize.x);
    int32_t y = floor(pos.y / mBucketSize.y);
    
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x > kMinimapBucketCount) x = kMinimapBucketCount;
    if(y > kMinimapBucketCount) y = kMinimapBucketCount;
    
    mBuckets[x][y] = true;
}

uint32_t Minimap::getDrawPriority() const
{
    return kMinimapPriority;
}

void Minimap::draw()
{
    Vector2 mapSize(64, 64);
    Vector2 mapOffset(208, 4);
    Vector2 mapSlice(mapSize / kMinimapBucketCount);
    
    mLifebar->draw();

    // Draw minimap.
    glLoadIdentity();
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
        points[j] = Vector2(mapSlice.x, 0) * Transform2::createRotation(M_PI/4*j) + mapSlice/2;
    glVertexPointer(2, GL_FLOAT, 0, (float*)points);
    
    glColor4f(.8, .15, .1, .4);
    for(uint32_t x=0; x < kMinimapBucketCount; ++x)
    {
        for(uint32_t y=0; y < kMinimapBucketCount; ++y)
        {
            if(!mBuckets[x][y]) continue;
            
            glLoadIdentity();
            glTranslatef(mapOffset.x + mapSlice.x*x, mapOffset.y + mapSlice.y*y, 0);
            glDrawArrays(GL_TRIANGLE_FAN, 0, num);
            mBuckets[x][y] = false;
        }
    }
    
    // Draw player.
    glColor4f(.1, .8, .1, .4);
    glLoadIdentity();
    Vector2 pos(floor(gWorld->getPlayer()->getCenter().x / mBucketSize.x) * mapSlice.x + mapOffset.x, floor(gWorld->getPlayer()->getCenter().y / mBucketSize.y) * mapSlice.y + mapOffset.y);
    glTranslatef(pos.x, pos.y, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num);
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

}