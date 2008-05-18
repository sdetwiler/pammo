#ifndef __MYVIEW_H__
#define __MYVIEW_H__

#include "sdlGraphicsContext.h"
#include "world.h"
#include "entity.h"

class MyView
{
public:
    MyView();
    virtual ~MyView();
    void init();

    void onUpdate();
    void drawRect();

    void pump();


protected:
    
    void buildWorld();
  


    World* mWorld;
    Entity* mEntity;
    bool mMoving;

    int mDownState; // 0:up 1:first down 2:still down 3:first up

    SdlGraphicsContext* mContext;

};
#endif // __MYVIEW_H__