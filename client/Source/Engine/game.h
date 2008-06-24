#ifndef __GAME_H__
#define __GAME_H__

#include "pammo.h"

namespace pammo
{

class Initable
{
public:
    virtual ~Initable(){}
    virtual void init() = 0;
};

class Deleteable
{
public:
    virtual ~Deleteable(){}
};

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
    virtual ~Drawable(){}
    virtual void draw() = 0;
    virtual uint32_t getDrawPriority() const = 0;
};

class Updateable
{
public:
    virtual ~Updateable(){}
    virtual void update() = 0;
};

class View
    : public Initable,
      public Deleteable,
      public Touchable,
      public Drawable,
      public Updateable
{
public:
    virtual ~View(){}
};

class Game
{
public:
    Game();
    virtual ~Game();

    int init();

    void touches(uint32_t count, Touch* touches);
    void update();

    void draw();
    
    void queueInitable(Initable* initable);
    void queueDeleteable(Deleteable* deleteable);

    void registerDrawable(Drawable* drawable);
    void unregisterDrawable(Drawable* drawable);

    void registerTouchable(Touchable* touchable);
    void unregisterTouchable(Touchable* tochable);

    void registerUpdateable(Updateable* updateable);
    void unregisterUpdateable(Updateable* updateable);
        
protected:

private:
    // Processes any pending initables or deleteables.
    void initAndDelete();

    typedef vector< Initable* > InitableVector;
    InitableVector mInitable;
    
    typedef vector< Deleteable* > DeleteableVector;
    DeleteableVector mDeleteable;

    typedef map< uint32_t, Drawable* > DrawableMap;
    DrawableMap mDrawable;
    
    typedef map< uint32_t, Touchable* > TouchableMap;
    TouchableMap mTouchable;

    typedef vector< Updateable* > UpdateableVector;
    UpdateableVector mUpdateable;
};
    
extern Game* gGame;

} // namespace pammo

#endif // __GAME_H__