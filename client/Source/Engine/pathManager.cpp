#include "pathManager.h"
#include "world.h"
#include "camera.h"
#include "collisionMap.h"

namespace pammo
{

PathManager::PathManager(Player* player)
{
    mPlayer = player;
    mBuilding = false;
    
    gGame->queueInitable(this);
}

PathManager::~PathManager()
{
    gGame->unregisterDrawable(this);
    gGame->unregisterTouchable(this);
}

void PathManager::init()
{
    gGame->registerDrawable(this);
    gGame->registerTouchable(this);
}

void PathManager::draw()
{
    if(mPoints.size() < 2)
        return;
	
	glLoadIdentity();
    gWorld->getCamera()->set();
    
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
    
    gWorld->getCamera()->unset();
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
        return true;

    case Touch::PhaseMove:
        mBuilding = true;
        addPoint(gWorld->getCamera()->translateToWorld(touches[0].mLocation));
        return true;
        break;

    case Touch::PhaseEnd:
        if(mBuilding)
        {
            gWorld->zoomIn();
            mPlayer->setPath(mPoints);
            clear();
            mBuilding = false;
            return true;
        }
    }

    return false;
}

void PathManager::addPoint(Vector2 const& point)
{
    Vector2 newPoint = point;
    if(mPoints.size() > 0)
    {
        Vector2 worldStart = mPoints[mPoints.size()-1];
        Vector2 worldEnd = point;
        vector< Vector2 > newPath;
        
        gWorld->getCollisionMap()->route(worldStart, worldEnd, 1, newPath);
        for(uint32_t i=0; i < newPath.size(); ++i)
            mPoints.push_back(newPath[i]);
    }
    else
    {
        mPoints.push_back(newPoint);
    }
}

void PathManager::clear()
{
    mPoints.clear();
}

//uint32_t PathManager::getDrawPriority() const
//{
//    return mDrawPriority;
//}
//
//void PathManager::setDrawPriority(uint32_t p)
//{
//    mDrawPriority = p;
//}
//
//uint32_t PathManager::getTouchPriority() const
//{
//    return mTouchPriority;
//}
//
//void PathManager::setTouchPriority(uint32_t p)
//{
//    mTouchPriority = p;
//}

} // namespace pammo.

