#ifndef __PATHMANAGER_H__
#define __PATHMANAGER_H__

#include "pammo.h"
#include "game.h"

namespace pammo
{

class PathManager : 
    public Drawable,
    public Touchable
{
public:
    PathManager();
    virtual ~PathManager();

    virtual void draw();
    virtual uint32_t getDrawPriority() const;
    virtual void setDrawPriority(uint32_t p);

    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getTouchPriority() const;
    virtual void setTouchPriority(uint32_t p);

protected:
    void addPoint(Vector2 const& point);
    void clear();

private:
    Vector2Vec mPoints;
    bool mBuilding;

    uint32_t mDrawPriority;
    uint32_t mTouchPriority;
};

}


#endif __PATHMANAGER_H__