#include "pammo.h"
#include "game.h"

#include "imageLibrary.h"
#include "view.h"

#include "world.h"
#include "mainScreenView.h"
namespace pammo
{
    
Game* gGame;

Game::Game()
{
    gGame = this;
    
    srand(time(NULL));
    
    // Initialize the image library.
    gImageLibrary = new ImageLibrary();
    
    //new VehicleSelectView();
    // Or create the world view with a map.
    new MainScreenView();
}

Game::~Game()
{
}

void Game::update()
{
    initAndDelete();
    
    for(ViewMap::iterator i=mUpdateable.begin(); i!=mUpdateable.end(); ++i)
    {
        i->second->update();
    }
}

void Game::draw()
{
    initAndDelete();
    
	Vector2 frame = getFrameSize();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    // Rotate coordinate system if we are on the iPhone.
#ifdef IPHONE
	glViewport(0, 0, (int)frame.y, (int)frame.x);
    glRotatef(-90, 0, 0, 1);
#else
	glViewport(0, 0, (int)frame.x, (int)frame.y);
#endif
    
	glOrthof(0, frame.x, frame.y, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    for(ViewMap::iterator i = mDrawable.begin(); i!= mDrawable.end(); ++i)
    {
        i->second->draw();
    }
}

void Game::touches(uint32_t count, Touch* touches)
{
    initAndDelete();
    
    for(ViewMap::iterator i=mTouchable.begin(); i!=mTouchable.end(); ++i)
    {
        if(i->second->touch(count, touches))
            return;
    }
}
    
void Game::queueInit(View* view)
{
    mInitable.push_back(view);
}

void Game::queueDelete(View* view)
{
    mDeleteable.push_back(view);
}
    
void Game::registerDrawable(View* view)
{
    assert(mDrawable.find(view->getDrawPriority()) == mDrawable.end());
    mDrawable.insert(ViewMap::value_type(view->getDrawPriority(), view));
}

void Game::unregisterDrawable(View* view)
{
    for(ViewMap::iterator i=mDrawable.begin(); i != mDrawable.end(); ++i)
    {
        if(i->second == view)
        {
            mDrawable.erase(i);
            return;
        }
    }
    assert(0);
}

void Game::registerTouchable(View* view)
{
    assert(mTouchable.find(view->getTouchPriority()) == mTouchable.end());
    mTouchable.insert(ViewMap::value_type(view->getTouchPriority(), view));
}

void Game::unregisterTouchable(View* view)
{
    for(ViewMap::iterator i=mTouchable.begin(); i != mTouchable.end(); ++i)
    {
        if(i->second == view)
        {
            mTouchable.erase(i);
            return;
        }
    }
    assert(0);
}

void Game::registerUpdateable(View* view)
{
    assert(mUpdateable.find(view->getUpdatePriority()) == mUpdateable.end());
    mUpdateable.insert(ViewMap::value_type(view->getUpdatePriority(), view));
}

void Game::unregisterUpdateable(View* view)
{
    for(ViewMap::iterator i=mUpdateable.begin(); i != mUpdateable.end(); ++i)
    {
        if(i->second == view)
        {
            mUpdateable.erase(i);
            return;
        }
    }
    assert(0);
}
    
void Game::initAndDelete()
{
    if(mInitable.size() == 0 && mDeleteable.size() == 0) return;
    
    ViewVector deleteableTmp = mDeleteable;
    mDeleteable.clear();
    for(ViewVector::iterator i = deleteableTmp.begin();  i != deleteableTmp.end(); ++i)
    {
        (*i)->unregister();
        delete *i;
    }
    
    ViewVector initableTmp = mInitable;
    mInitable.clear();
    for(ViewVector::iterator i = initableTmp.begin(); i != initableTmp.end(); ++i)
    {
        (*i)->init();
    }
}

}
