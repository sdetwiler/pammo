#include "game.h"
#include "builder.h"
#include "world.h"
#include "pathWidget.h"

Game::Game()
{

}

Game::~Game()
{
}

int Game::init()
{
    World* world = new World(this);
    registerDrawable(world);
    registerTouchable(world);
    registerUpdateable(world);

    builder(world);

    PathWidget* pathWidget = new PathWidget;
    pathWidget->setDrawPriority(100);
    pathWidget->setTouchPriority(1);
    registerDrawable(pathWidget);
    registerTouchable(pathWidget);


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
        int* a;
        *a = 0;
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