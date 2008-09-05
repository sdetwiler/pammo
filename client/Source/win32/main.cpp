#include "pammo.h"
#include "game.h"

#include "world.h"
#include "player.h"
#include "enemyManager.h"

Timer gTimer;

class InputProcessor
{
public:
    InputProcessor()
    {
        memset(&mDown, 0, sizeof(mDown));
        keyTouch[0].mLocation.x = 60;
        keyTouch[0].mLocation.y = 260;
        keyTouch[1].mLocation.x = 420;
        keyTouch[1].mLocation.y = 260;

    }

    ~InputProcessor()
    {
    }

    Touch keyTouch[2];
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
            bool directionKey = false;
            bool fireKey = false;
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







                else if(mKeySym.sym == SDLK_LEFT)
                {
                    keyTouch[1].mPhase = Touch::PhaseBegin;
                    keyTouch[1].mLocation.x += -keyRange;
                    keyTouch[1].mLocation.y += 0;
                    fireKey = true;
                }
                else if(mKeySym.sym == SDLK_RIGHT)
                {
                    keyTouch[1].mPhase = Touch::PhaseBegin;
                    keyTouch[1].mLocation.x += keyRange;
                    keyTouch[1].mLocation.y += 0;
                    fireKey = true;
                }

                else if(mKeySym.sym == SDLK_UP)
                {
                    keyTouch[1].mPhase = Touch::PhaseBegin;
                    keyTouch[1].mLocation.x += 0;
                    keyTouch[1].mLocation.y += -keyRange;
                    fireKey = true;
                }
                else if(mKeySym.sym == SDLK_DOWN)
                {
                    keyTouch[1].mPhase = Touch::PhaseBegin;
                    keyTouch[1].mLocation.x += 0;
                    keyTouch[1].mLocation.y += keyRange;
                    fireKey = true;
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



                else if(mKeySym.sym == SDLK_LEFT)
                {
                    keyTouch[1].mPhase = Touch::PhaseEnd;
                    keyTouch[1].mLocation.x += keyRange;
                    keyTouch[1].mLocation.y += 0;
                    fireKey = true;
                }
                else if(mKeySym.sym == SDLK_RIGHT)
                {
                    keyTouch[1].mPhase = Touch::PhaseEnd;
                    keyTouch[1].mLocation.x += -keyRange;
                    keyTouch[1].mLocation.y += 0;
                    fireKey = true;
                }

                else if(mKeySym.sym == SDLK_UP)
                {
                    keyTouch[1].mPhase = Touch::PhaseEnd;
                    keyTouch[1].mLocation.x += 0;
                    keyTouch[1].mLocation.y += keyRange;
                    fireKey = true;
                }
                else if(mKeySym.sym == SDLK_DOWN)
                {
                    keyTouch[1].mPhase = Touch::PhaseEnd;
                    keyTouch[1].mLocation.x += 0;
                    keyTouch[1].mLocation.y += -keyRange;
                    fireKey = true;
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
//				dprintf("### %.2f %.2f", keyTouch[0].mLocation.x, keyTouch[0].mLocation.y);
				game->touches(1, keyTouch);
            }
            if(fireKey)
            {
				cap(&keyTouch[1].mLocation.x, 380, 460);
				cap(&keyTouch[1].mLocation.y, 220, 300);
//				dprintf("### %.2f %.2f", keyTouch[1].mLocation.x, keyTouch[1].mLocation.y);
				game->touches(1, keyTouch+1);
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
    // 1 ms accuracy for sleep.
    timeBeginPeriod(1);

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
    SDL_WM_SetCaption("Irradiated", NULL);

    Game* game = new Game;

    InputProcessor input;

    uint64_t start = getTime();
    uint64_t lastCheck = start;
    uint64_t now;
    uint32_t frames = 0;
    while(true)
    {
        uint64_t a = getTime();

        input.update(game);
        game->update();
        game->draw();
        SDL_GL_SwapBuffers();
        ++frames;

        now = getTime();


        float delta = ((now-a)/10000.0f);
        //dprintf("delta: %.2f", delta);
        if(delta>33.0f)
        {
            delta = 33.0f;
        }

        Sleep((DWORD)(33.0f-delta));

        if(now - lastCheck > 2000000)
        {
            float delta = now-lastCheck;
            float seconds = (delta)/1000000.0f;
            dprintf("Runtime: %.2f\tFPS: %.2f\tscore: %u\tnext wave: %u", 
                ((float)now-(float)start)/1000000.0f, 
                (float)frames/seconds,
                gWorld?(gWorld->getPlayer()?gWorld->getPlayer()->mScore:0):0,
                gWorld?(gWorld->getEnemyManager()?gWorld->getEnemyManager()->mNextWaveScore:0):0
                );
            frames = 0;
            lastCheck = now;
        }
    }

    delete game;

    return 0;
}
