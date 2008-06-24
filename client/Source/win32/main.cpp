#include "pammo.h"
#include "game.h"
#include "image.h"

class InputProcessor
{
public:
    InputProcessor()
    {
        memset(&mDown, 0, sizeof(mDown));
    }

    ~InputProcessor()
    {
    }

    SDL_keysym mKeySym;
    bool mDown[5];

    void InputProcessor::update(Game* game)
    {
        SDL_Event event;
        Vector2 location;
        Touch touch[5];
        bool altUp = false;
        bool mouseUp = false;
        while(SDL_PollEvent(&event)) 
        {
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
                break;

            case SDL_KEYUP:
                memset(&mKeySym, 0, sizeof(mKeySym));
                if(mKeySym.sym == SDLK_RALT || mKeySym.sym== SDLK_LALT)
                {
                    touch[1].mPhase = Touch::PhaseEnd;
                    touch[1].mLocation = touch[0].mLocation;
                    altUp = true;
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
                break;

            case SDL_QUIT:
                exit(0);
            }// switch

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



int main(int argc, char *argv[]) 
{
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
    ret = game->init(); 
    if(ret < 0)
    {
        return ret;
    }

    InputProcessor input;

    // 1 ms accuracy for timer.
    timeBeginPeriod(5);
    DWORD start = timeGetTime();
    DWORD now;
    uint32_t frames = 0;
    while(true)
    {
        input.update(game);

        game->update();

        game->draw();
        SDL_GL_SwapBuffers();
        ++frames;

        Sleep(33);

        now = timeGetTime();
        
        if(now - start > 5000)
        {
            dprintf("%.2f fps", (float)(now-start)/(float)frames);
            frames = 0;
            start = now;
        }
    }

    delete game;

    return 0;
}
