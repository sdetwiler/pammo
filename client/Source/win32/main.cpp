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
    SDL_keysym mKeySym;

    void InputProcessor::update(Game* game)
    {
        SDL_Event event;
        Vector2 location;
        Touch touch[2];

        while(SDL_PollEvent(&event)) 
        {
            switch(event.type) 
            {
            case SDL_KEYDOWN:
                mKeySym = event.key.keysym;
                break;

            case SDL_KEYUP:
                memset(&mKeySym, 0, sizeof(mKeySym));
                break;

            case SDL_MOUSEMOTION:
                if(mDownState == 0)
                    break;

                 // If this was the first down, advance to still down and start touch
                if(mDownState == 1)
                {
                    touch[0].mPhase = Touch::PhaseBegin;
                    mDownState = 2;
                }
                // If still down, touch move
                else if(mDownState == 2)
                {
                    touch[0].mPhase = Touch::PhaseMove;
                }

                touch[0].mLocation.x = event.motion.x;
                touch[0].mLocation.y = event.motion.y;
                
                // Tell the game it's being touched.
                  
                
                // Two finger hack.
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    // SCD: This is wrong. We need to generate a touch end if only alt is released.
                    touch[1].mLocation = touch[0].mLocation;
                    touch[1].mPhase = touch[0].mPhase;
                    game->touches(2, touch);
                }
                else
                {
                    game->touches(1, touch);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                touch[0].mLocation.x = event.motion.x;
                touch[0].mLocation.y = event.motion.y;
                touch[0].mPhase = Touch::PhaseBegin;
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    dprintf("two finger touch/click\n");
                    touch[1].mLocation.x = event.motion.x;
                    touch[1].mLocation.y = event.motion.y;
                    touch[1].mPhase = Touch::PhaseBegin;
                    game->touches(2, touch);

                }
                else
                {
                    dprintf("touch/click\n");
                    game->touches(1, touch);
                }


                mDownState = 1;
                break;

            case SDL_MOUSEBUTTONUP:
                touch[0].mLocation.x = event.motion.x;
                touch[0].mLocation.y = event.motion.y;
                touch[0].mPhase = Touch::PhaseEnd;
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    dprintf("two finger touch/click\n");
                    touch[1].mLocation.x = event.motion.x;
                    touch[1].mLocation.y = event.motion.y;
                    touch[1].mPhase = Touch::PhaseEnd;
                    game->touches(2, touch);

                }
                else
                {
                    dprintf("touch/click\n");
                    game->touches(1, touch);
                }
                mDownState = 0;

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
