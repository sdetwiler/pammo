#ifndef __GAME_H__
#define __GAME_H__

#include "pammo.h"

namespace pammo
{

class Touchable
{
public:
    virtual ~Touchable(){}
    virtual bool touch(uint32_t count, Touch* touches) = 0;
    virtual uint32_t getTouchPriority() const = 0;
};

class Drawable
{
public:
    Drawable(){}
    virtual ~Drawable(){}
    virtual void draw() = 0;
    virtual uint32_t getDrawPriority() const = 0;

private:
};

class Updateable
{
public:
    virtual ~Updateable(){}
    virtual void update(int delta) = 0;
};

class Game
{
public:
    Game();
    virtual ~Game();

    int init();

    void touches(uint32_t count, Touch* touches);
    void update(int delta);

    void draw();

    void registerDrawable(Drawable* drawable);
    void unregisterDrawable(Drawable* drawable);

    void registerTouchable(Touchable* touchable);
    void unregisterTouchable(Touchable* tochable);

    void registerUpdateable(Updateable* updateable);
    void unregisterUpdateable(Updateable* updateable);
        
protected:

private:
    typedef map< uint32_t, Drawable* > DrawableMap;
    DrawableMap mDrawable;
    
    typedef map< uint32_t, Touchable* > TouchableMap;
    TouchableMap mTouchable;

    typedef vector< Updateable* > UpdateableVector;
    UpdateableVector mUpdateable;
};

} // namespace pammo

#endif // __GAME_H__