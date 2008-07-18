#ifndef __GAME_H__
#define __GAME_H__

namespace pammo
{

class View;

class Game
{
public:
    Game();
    ~Game();

    void touches(uint32_t count, Touch* touches);
    void update();
    void draw();
        
protected:
    friend class View;
    
    void queueInit(View* view);
    void queueDelete(View* view);

    void registerDrawable(View* view);
    void unregisterDrawable(View* view);

    void registerTouchable(View* view);
    void unregisterTouchable(View* view);

    void registerUpdateable(View* view);
    void unregisterUpdateable(View* view);

private:
    // Processes any pending initables or deleteables.
    void initAndDelete();

    typedef vector< View* > ViewVector;
    typedef map< uint32_t, View* > ViewMap;
    
    ViewVector mInitable;
    ViewVector mDeleteable;
    
    ViewMap mDrawable;
    ViewMap mTouchable;
    ViewMap mUpdateable;
};
    
extern Game* gGame;

} // namespace pammo

#endif // __GAME_H__