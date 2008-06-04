#include "game.h"
#include "image.h"
#include "imageLibrary.h"
#include "lobbyView.h"

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
    
    // Create the lobby view.
    new LobbyView();

    return 0;
}

void Game::update(int delta)
{
    for(UpdateableVector::iterator i=mUpdateable.begin(); i!=mUpdateable.end(); ++i)
    {
        (*i)->update(delta);
    }
}

void Game::draw()
{
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
    for(TouchableMap::iterator i=mTouchable.begin(); i!=mTouchable.end(); ++i)
    {
        if(i->second->touch(count, touches))
            return;
    }
}


void Game::registerDrawable(Drawable* drawable)
{
    pair<DrawableMap::iterator, bool> i = mDrawable.insert(DrawableMap::value_type(drawable->getDrawPriority(), drawable));
    if(i.second == false)
    {
        assert(0);
    }
}

void Game::unregisterDrawable(Drawable* drawable)
{
    // Linear seeky.

}

void Game::registerTouchable(Touchable* touchable)
{
    mTouchable.insert(TouchableMap::value_type(touchable->getTouchPriority(), touchable));
}

void Game::unregisterTouchable(Touchable* touchable)
{
    // suck, a linear seek.
}

void Game::registerUpdateable(Updateable* updateable)
{
    mUpdateable.push_back(updateable);
}

void Game::unregisterUpdateable(Updateable* updateable)
{
// 
}

}
