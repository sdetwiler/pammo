#include "types_platform.h"
#include "pathWidget.h"
#include "world.h"

namespace pammo
{

PathWidget::PathWidget(World* world)
{
    mWorld = world;
    mBuilding = false;
}

PathWidget::~PathWidget()
{

}

void PathWidget::draw()
{
    if(mPoints.size() < 2)
        return;

    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_LINE_SMOOTH);

    glLineWidth(5.0);
    glColor4f(1.0, 0.0, 0.0, 0.5);

    glBegin(GL_LINE_STRIP);
    for(Vector2Vec::iterator i = mPoints.begin(); i!=mPoints.end(); ++i)
    {
        glVertex3f((*i).x, (*i).y, 0.0);
    }
    glEnd();

    glDisable(GL_LINE_SMOOTH);

    glPopMatrix();
}

bool PathWidget::touch(uint32_t count, Touch* touches)
{
    bool ret = false;
    // Always only use first touch point.
    switch(touches[0].mPhase)
    {
    case Touch::PhaseBegin:
    case Touch::PhaseMove:
        mBuilding = true;
        addPoint(touches[0].mLocation);
        ret = true;
        break;

    case Touch::PhaseEnd:
        if(mBuilding)
        {
            mWorld->setPath(mPoints);
            clear();
            mBuilding = false;
            ret = true;
        }
        break;
    }

    return ret;
}

void PathWidget::update(int delta)
{

}

void PathWidget::addPoint(Vector2 const& point)
{
    mPoints.push_back(point);
}

void PathWidget::clear()
{
    mPoints.clear();
}

}