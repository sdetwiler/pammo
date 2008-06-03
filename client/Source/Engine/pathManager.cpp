#include "pathManager.h"
#include "world.h"

namespace pammo
{

PathManager::PathManager()
{
    mBuilding = false;
    mDrawPriority = 100;
    mTouchPriority = 1;
    
    gGame->registerDrawable(this);
    gGame->registerTouchable(this);
}

PathManager::~PathManager()
{

}

void PathManager::draw()
{
    if(mPoints.size() < 2)
        return;
	
	glLoadIdentity();
    glLineWidth(5.0);
    glColor4f(1.0, 0.0, 0.0, 0.5);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	uint32_t cur=0;
	float* tmp = new float[mPoints.size()*2];
    for(Vector2Vec::iterator i = mPoints.begin(); i!=mPoints.end(); ++i)
    {
		tmp[cur] = i->x;
		tmp[cur + 1] = i->y;
		cur += 2;
    }
	glVertexPointer(2, GL_FLOAT, 0, tmp);
	
	glDrawArrays(GL_LINE_STRIP, 0, mPoints.size());
	
	delete[] tmp;
	
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool PathManager::touch(uint32_t count, Touch* touches)
{
    if(count != 1)
        return false;

    if(gWorld->isZoomedOut() == false)
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
            gWorld->setPath(mPoints);
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

