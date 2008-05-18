#include "myView.h"

#include "world.h"
#include "camera.h"
#include "entity.h"
#include "builder.h"


MyView::MyView()
{
    mDownState = 0;
}

MyView::~MyView()
{
    if(mWorld)
        delete mWorld;

    if(mContext)
        delete mContext;
}

void MyView::init()
{
    mContext = new SdlGraphicsContext;
    buildWorld();
  }

void MyView::buildWorld()
{
    mWorld = new World(mContext);
	mEntity = 0;

	builder(mWorld);
}

void MyView::drawRect(/* rect */)
{
	mWorld->draw();
}

void MyView::onUpdate()
{
}

void MyView::pump()
{
    SDL_Event event;
    Touch touch;

    while(SDL_PollEvent(&event)) 
    {
        switch(event.type) 
        {
            case SDL_MOUSEMOTION:
                if(mDownState == 0)
                    break;

                 // If this was the first down, advance to still down and start touch
                if(mDownState == 1)
                {
                    touch.mPhase = Touch::TouchPhaseBegin;
                    mDownState = 2;
                }
                // If still down, touch move
                else if(mDownState == 2)
                {
                    touch.mPhase = Touch::TouchPhaseMove;
                }
                // If just up, touch end.
                else if(mDownState == 3)
                {
                    touch.mPhase = Touch::TouchPhaseEnd;
                }
                touch.mLocation.x = event.motion.x;
                touch.mLocation.y = event.motion.y;
                mWorld->touches(1, &touch);
                break;

            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button %d pressed at (%d,%d)\n",
                       event.button.button, event.button.x, event.button.y);

                mDownState = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                mDownState = 3;
                break;

            case SDL_QUIT:
                exit(0);
        }
    }
}