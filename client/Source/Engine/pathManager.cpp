#include "pathManager.h"
#include "world.h"

namespace pammo
{

PathManager::PathManager(World* world)
{
    mWorld = world;
    mBuilding = false;
    mDrawPriority = 1;
    mTouchPriority = 1;
}

PathManager::~PathManager()
{

}

void PathManager::draw()
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

bool PathManager::touch(uint32_t count, Touch* touches)
{
    if(count != 1)
        return false;

    if(mWorld->isZoomedOut() == false)
        return false;

    // Always only use first touch point.
    switch(touches[0].mPhase)
    {
    case Touch::PhaseBegin:
        return false;

    case Touch::PhaseMove:
        mBuilding = true;
        addPoint(touches[0].mLocation);
        return true;
        break;

    case Touch::PhaseEnd:
        if(mBuilding)
        {
            mWorld->setPath(mPoints);
            clear();
            mBuilding = false;
            return true;
        }
    }

    return false;
}

void PathManager::addPoint(Vector2 const& point)
{
    mPoints.push_back(point);
}

void PathManager::clear()
{
    mPoints.clear();
}

uint32_t PathManager::getDrawPriority() const
{
    return mDrawPriority;
}

void PathManager::setDrawPriority(uint32_t p)
{
    mDrawPriority = p;
}

uint32_t PathManager::getTouchPriority() const
{
    return mTouchPriority;
}

void PathManager::setTouchPriority(uint32_t p)
{
    mTouchPriority = p;
}

} // namespace pammo.

