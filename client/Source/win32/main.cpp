#include "pammo.h"
#include "game.h"

Timer gTimer;

class InputProcessor
{
public:
    InputProcessor()
    {
        memset(&mDown, 0, sizeof(mDown));
        keyTouch[0].mLocation.x = 60;
        keyTouch[0].mLocation.y = 260;
    }

    ~InputProcessor()
    {
    }

    Touch keyTouch[1];
    SDL_keysym mKeySym;
    bool mDown[5];


    void cap(float* val, float min, float max)
    {
        if(*val < min)
            *val = min;
        else if(*val > max)
            *val = max;
    }

    void InputProcessor::update(Game* game)
    {
        SDL_Event event;
        Vector2 location;
        Touch touch[5];
        float keyRange = 40;
        

        bool altUp = false;
        bool mouseUp = false;
        while(SDL_PollEvent(&event)) 
        {
            float directionKey = false;
            switch(event.type) 
            {
            case SDL_KEYDOWN:
                mKeySym = event.key.keysym;
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    touch[1].mPhase = Touch::PhaseBegin;
                    touch[1].mLocation = touch[0].mLocation;
                    mDown[1] = true;
                }

                // Keyboard input for movement control.
                else if(mKeySym.sym == SDLK_a)
                {
                    keyTouch[0].mPhase = Touch::PhaseBegin;
                    keyTouch[0].mLocation.x += -keyRange;
                    keyTouch[0].mLocation.y += 0;
                    directionKey = true;
                }
                else if(mKeySym.sym == SDLK_d)
                {
                    keyTouch[0].mPhase = Touch::PhaseBegin;
                    keyTouch[0].mLocation.x += keyRange;
                    keyTouch[0].mLocation.y += 0;
                    directionKey = true;
                }

                else if(mKeySym.sym == SDLK_w)
                {
                    keyTouch[0].mPhase = Touch::PhaseBegin;
                    keyTouch[0].mLocation.x += 0;
                    keyTouch[0].mLocation.y += -keyRange;
                    directionKey = true;
                }
                else if(mKeySym.sym == SDLK_s)
                {
                    keyTouch[0].mPhase = Touch::PhaseBegin;
                    keyTouch[0].mLocation.x += 0;
                    keyTouch[0].mLocation.y += keyRange;
                    directionKey = true;
                }

                break;

            case SDL_KEYUP:
                //memset(&mKeySym, 0, sizeof(mKeySym));
                mKeySym = event.key.keysym;
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    touch[1].mPhase = Touch::PhaseEnd;
                    touch[1].mLocation = touch[0].mLocation;
                    altUp = true;
                }


                // Keyboard input for movement control.
                else if(mKeySym.sym == SDLK_a)
                {
                    keyTouch[0].mPhase = Touch::PhaseEnd;
                    keyTouch[0].mLocation.x += keyRange;
                    keyTouch[0].mLocation.y += 0;
                    directionKey = true;
                }
                else if(mKeySym.sym == SDLK_d)
                {
                    keyTouch[0].mPhase = Touch::PhaseEnd;
                    keyTouch[0].mLocation.x += -keyRange;
                    keyTouch[0].mLocation.y += 0;
                    directionKey = true;
                }

                else if(mKeySym.sym == SDLK_w)
                {
                    keyTouch[0].mPhase = Touch::PhaseEnd;
                    keyTouch[0].mLocation.x += 0;
                    keyTouch[0].mLocation.y += keyRange;
                    directionKey = true;
                }
                else if(mKeySym.sym == SDLK_s)
                {
                    keyTouch[0].mPhase = Touch::PhaseEnd;
                    keyTouch[0].mLocation.x += 0;
                    keyTouch[0].mLocation.y += -keyRange;
                    directionKey = true;
                }




                break;

            case SDL_MOUSEMOTION:
                if(mDown[0] == false)
                    break;
                
                if(mDown[0])
                {
                    touch[0].mPhase = Touch::PhaseMove;
                    touch[0].mLocation.x = event.motion.x;
                    touch[0].mLocation.y = event.motion.y;
                }

                if(mDown[1])
                {
                    touch[1].mPhase = Touch::PhaseMove;
                    touch[1].mLocation.x = event.motion.x;
                    touch[1].mLocation.y = event.motion.y;
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    mDown[0] = true;
                    touch[0].mPhase = Touch::PhaseBegin;
                    touch[0].mLocation.x = event.button.x;
                    touch[0].mLocation.y = event.button.y;
                    if(mDown[1])
                    {
                        touch[1].mPhase = Touch::PhaseBegin;
                        touch[1].mLocation.x = event.button.x;
                        touch[1].mLocation.y = event.button.y;
                    }
                }
                else if(SDL_BUTTON_RIGHT)
                {
                    touch[0].mPhase = Touch::PhaseBegin;
                    touch[0].mLocation = touch[0].mLocation;
                    mDown[0] = true;
                    
                    touch[1].mPhase = Touch::PhaseBegin;
                    touch[1].mLocation = touch[0].mLocation;
                    mDown[1] = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mouseUp = true;
                touch[0].mPhase = Touch::PhaseEnd;
                touch[0].mLocation.x = event.button.x;
                touch[0].mLocation.y = event.button.y;
                if(mDown[1])
                {
                    touch[1].mPhase = Touch::PhaseEnd;
                    touch[1].mLocation.x = event.button.x;
                    touch[1].mLocation.y = event.button.y;
                }

                else if(SDL_BUTTON_RIGHT)
                {
                    touch[0].mPhase = Touch::PhaseEnd;
                    touch[0].mLocation.x = event.button.x;
                    touch[0].mLocation.y = event.button.y;
                    
                    touch[1].mPhase = Touch::PhaseEnd;
                    touch[1].mLocation.x = event.button.x;
                    touch[1].mLocation.y = event.button.y;
                }

                break;

            case SDL_QUIT:
                exit(0);
            }// switch


            if(directionKey)
            {
				cap(&keyTouch[0].mLocation.x, 20, 100);
				cap(&keyTouch[0].mLocation.y, 220, 300);
				dprintf("### %.2f %.2f", keyTouch[0].mLocation.x, keyTouch[0].mLocation.y);
				game->touches(1, keyTouch);
            }

            if(mDown[0] && mDown[1])
            {
                if(mouseUp)
                {
                    touch[1].mPhase = Touch::PhaseEnd;
                }

                game->touches(2, touch);

                if(mouseUp)
                {
                    mDown[0] = false;
                    mDown[1] = false;
                }

                if(altUp)
                {
                    mDown[1] = false;
                }
            }
            else if(mDown[0] && !mDown[1])
            {
                game->touches(1, touch);
                if(mouseUp)
                {
                    mDown[0] = false;
                }
            }
        }// while    
    }
};// class


int initTimer(Timer* timer)
{
    _ftime(&timer->timeBuffer);
    timer->startTime = (uint64_t)timer->timeBuffer.time * 1000000 + (uint64_t)timer->timeBuffer.millitm * 1000;
    if(!QueryPerformanceFrequency(&timer->freq))
    {
        dprintf("QueryPerformanceFrequency failed");
        return -1;
    }
    if(!QueryPerformanceCounter(&timer->now))
    {
        dprintf("QueryPerformanceCounter failed");
        return -1;
    }
    
    return 0;
}

int main(int argc, char *argv[]) 
{
    if(initTimer(&gTimer) < 0)
    {
        return -1;
    }

    char path[256];
    if(getcwd(path, sizeof(path)) == NULL)
        return -1;

    if(strstr(path, "releases"))
    {
        dprintf("In releases directory, setting current directory two levels up.\n");
        chdir("..\\..\\");
    }
    else
    {
        dprintf("Not changing current working directory.\n");
    }
           

    SDL_Surface* screen;
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode((int)getFrameSize().x, (int)getFrameSize().y, 0, SDL_OPENGL);
    SDL_WM_SetCaption("PAMMO", NULL);

    int ret;
    Game* game = new Game;

    InputProcessor input;

    // 1 ms accuracy for timer.
    timeBeginPeriod(5);
    uint64_t start = getTime();
    uint64_t now;
    uint32_t frames = 0;
    while(true)
    {
        input.update(game);

        game->update();

        game->draw();
        SDL_GL_SwapBuffers();
        ++frames;

        Sleep(33);

        now = getTime();
        
        if(now - start > 2000000)
        {
            float delta = now-start;
            float seconds = (delta)/1000000.0f;
            dprintf("%.2f FPS", (float)frames/seconds);

            frames = 0;
            start = now;
        }
    }

    delete game;

    return 0;
}
