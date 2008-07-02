#include "npcManager.h"
#include "pathManager.h"
#include "player.h"
#include "world.h"
#include "collisionMap.h"

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
        ret = mPlayers[i].init(Player::Remote);
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
    
    //path.push_back(Vector2(100, 100));
    //path.push_back(Vector2(-100, 140));
    //path.push_back(Vector2(100, 100));
    //path.push_back(Vector2(-100, 140));
    //player->setPath(path);
    //return;

    Vector2 lastPos;
    Vector2 newPos;

    lastPos = player->getCenter();

    int max = 800;
    for(uint32_t i=0; i<5; ++i)
    {
        newPos.x =(rand()%max);//-(max/2); 
        newPos.y =(rand()%max);//-(max/2); 

        Vector2Vec newPath;
        if(gWorld->getCollisionMap()->route(lastPos, newPos, 20, newPath))
        {
            for(Vector2Vec::iterator j = newPath.begin(); j!=newPath.end(); ++j)
                path.push_back( *j );
        }
        else
            path.push_back(newPos);

        lastPos = newPos;
    }
    dprintf("");
    player->setPath(path);
}

void NpcManager::update()
{
    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        if(mPlayers[i].getState() == Player::Alive || mPlayers[i].getState() == Player::Spawning)
        {
            if(mPlayers[i].isMoving() == false)
            {
                generatePath(mPlayers+i);
            }
        }        
        mPlayers[i].update();
    }
}

void NpcManager::draw()
{
    for(uint32_t i=0; i<mNumPlayers; ++i)
    {
        if(mPlayers[i].getState() == Player::Alive || mPlayers[i].getState() == Player::Spawning)
        {
            mPlayers[i].draw();
        }
    }
}
Player* NpcManager::hitTest(Vector2 startPos, Vector2 endPos, float radius)
{
    //gWorld->getCollisionMap()->myMagicFunction();
    return NULL;
}


} // namespace pammo