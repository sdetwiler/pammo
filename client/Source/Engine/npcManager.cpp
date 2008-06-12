#include "npcManager.h"
#include "pathManager.h"
#include "player.h"

namespace pammo
{

NpcManager::NpcManager()
{
    mPlayers = NULL;
    mNumPlayers = 0;
}

NpcManager::~NpcManager()
{
    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        delete &mPlayers[i];
    }
}

int NpcManager::init(uint32_t numPlayers)
{
    int ret;
    mNumPlayers = numPlayers;
    mPlayers = new Player[mNumPlayers];

    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        ret = mPlayers[i].init();
        if(ret < 0)
        {
            delete mPlayers;
            mNumPlayers = 0;
        }
    }

    return 0;
}

void NpcManager::generatePath(Player* player)
{
    Vector2Vec path;

// BRAIN DEAD!
    int max = 400;
    for(uint32_t i=0; i<5; ++i)
    {
        float x = (rand()%max)-(max/2) + 300;
        float y = (rand()%max)-(max/2) + 300;

        dprintf("x: %.2f y: %.2f", x, y);
        path.push_back( Vector2(x,y) );
    }
    dprintf("");
    player->setPath(path);
}

void NpcManager::update()
{
    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        if(mPlayers[i].isMoving() == false)
        {
            generatePath(mPlayers+i);
        }
        
        mPlayers[i].update();
    }
}

void NpcManager::draw()
{
    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        mPlayers[i].draw();
    }
}
} // namespace pammo