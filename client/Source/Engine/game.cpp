#include "game.h"
#include "image.h"
#include "imageLibrary.h"
#include "lobbyView.h"
#include "world.h"

namespace pammo
{
    
Game* gGame;

Game::Game()
{
    gGame = this;
}

Game::~Game()
{
}

int Game::init()
{
    // Let the image subroutine precompile vert arrays.
    initImage();
    
    // Initialize the image library.
    gImageLibrary = new ImageLibrary();
    
#if 0
    // Create the lobby view.
    new LobbyView();
#else
    // Or create the world view with a map.
    new World("Desert");
#endif

    return 0;
}

void Game::update(int delta)
{
    initAndDelete();
    
    for(UpdateableVector::iterator i=mUpdateable.begin(); i!=mUpdateable.end(); ++i)
    {
        (*i)->update(delta);
    }
}

void Game::draw()
{
    initAndDelete();
    
	Vector2 frame = getFrameSize();
	
	glViewport(0, 0, (int)frame.x, (int)frame.y);
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
	glOrthof(0, frame.x, frame.y, 0, -1.0, 1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
    for(DrawableMap::iterator i = mDrawable.begin(); i!= mDrawable.end(); ++i)
    {
        i->second->draw();
    }
}

void Game::touches(uint32_t count, Touch* touches)
{
    initAndDelete();
    
    for(TouchableMap::iterator i=mTouchable.begin(); i!=mTouchable.end(); ++i)
    {
        if(i->second->touch(count, touches))
            return;
    }
}
    
void Game::queueInitable(Initable* initable)
{
    mInitable.push_back(initable);
}

void Game::queueDeleteable(Deleteable* deleteable)
{
    mDeleteable.push_back(deleteable);
}
    
void Game::registerDrawable(Drawable* drawable)
{
    mDrawable.insert(DrawableMap::value_type(drawable->getDrawPriority(), drawable));
}

void Game::unregisterDrawable(Drawable* drawable)
{
    for(DrawableMap::iterator i=mDrawable.begin(); i != mDrawable.end(); ++i)
    {
        if(i->second == drawable)
        {
            mDrawable.erase(i);
            return;
        }
    }
    assert(0);
}

void Game::registerTouchable(Touchable* touchable)
{
    mTouchable.insert(TouchableMap::value_type(touchable->getTouchPriority(), touchable));
}

void Game::unregisterTouchable(Touchable* touchable)
{
    for(TouchableMap::iterator i=mTouchable.begin(); i != mTouchable.end(); ++i)
    {
        if(i->second == touchable)
        {
            mTouchable.erase(i);
            return;
        }
    }
    assert(0);
}

void Game::registerUpdateable(Updateable* updateable)
{
    mUpdateable.push_back(updateable);
}

void Game::unregisterUpdateable(Updateable* updateable)
{
    for(UpdateableVector::iterator i=mUpdateable.begin(); i != mUpdateable.end(); ++i)
    {
        if(*i == updateable)
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
    
    DeleteableVector deleteableTmp = mDeleteable;
    mDeleteable.clear();
    for(DeleteableVector::iterator i = deleteableTmp.begin();  i != deleteableTmp.end(); ++i)
    {
        delete *i;
    }
    
    InitableVector initableTmp = mInitable;
    mInitable.clear();
    for(InitableVector::iterator i = initableTmp.begin(); i != initableTmp.end(); ++i)
    {
        (*i)->init();
    }
}

}
