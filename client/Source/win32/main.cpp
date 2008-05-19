#include "pammo.h"
#include "game.h"
#include "image.h"

class InputProcessor
{
public:
    InputProcessor()
    {
        mDownState = 0;
    }

    ~InputProcessor()
    {
    }

    int mDownState;

    void InputProcessor::update(Game* game)
    {
        SDL_Event event;
        Touch touch;
        Vector2 location;

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
                        touch.mPhase = Touch::PhaseBegin;
                        mDownState = 2;
                    }
                    // If still down, touch move
                    else if(mDownState == 2)
                    {
                        touch.mPhase = Touch::PhaseMove;
                    }
                    // If just up, touch end.
                    else if(mDownState == 3)
                    {
                        touch.mPhase = Touch::PhaseEnd;
                    }

                    touch.mLocation.x = event.motion.x;
                    touch.mLocation.y = event.motion.y;
                    
                    // Tell the game it's being touched.
                    game->touches(1, &touch);

                    break;

                case SDL_MOUSEBUTTONDOWN:
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
};

int main(int argc, char *argv[]) 
{
    SDL_Surface* screen;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(getFrameSize().x, getFrameSize().y, 0, SDL_OPENGL);
    SDL_WM_SetCaption("PAMMO", NULL);

    glViewport(0, 0, getFrameSize().x, getFrameSize().y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, getFrameSize().x, getFrameSize().y, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int ret;
    Game* game = new Game;
    ret = game->init(); 
    if(ret < 0)
    {
        return ret;
    }

    InputProcessor input;

    while(true)
    {
        input.update(game);

        game->update(1);

        glClear(GL_COLOR_BUFFER_BIT);		// Clear The Screen
        game->draw();
        SDL_GL_SwapBuffers();

        Sleep(1000/30);
    }

    delete game;

    return 0;
}
