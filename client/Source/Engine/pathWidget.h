#ifndef __PATHWIDGET_H__
#define __PATHWIDGET_H__

#include "pammo.h"
#include "widget.h"

namespace pammo
{

class World;

class PathWidget : 
    public Widget
{
public:
    PathWidget(World* world);
    virtual ~PathWidget();

    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update(int delta);

protected:
    void addPoint(Vector2 const& point);
    void clear();

private:
    World* mWorld;
    Vector2Vec mPoints;
    bool mBuilding;
};

}


#endif __PATHWIDGET_H__