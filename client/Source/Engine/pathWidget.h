#ifndef __PATHWIDGET_H__
#define __PATHWIDGET_H__

#include "widget.h"

class PathWidget : 
    public Widget
{
public:
    PathWidget();
    virtual ~PathWidget();

    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual void update(int delta);

protected:
    void addPoint(Vector2 const& point);
    void clear();

private:
    typedef std::vector< Vector2 > Vector2Vec;
    Vector2Vec mPoints;
};


#endif __PATHWIDGET_H__