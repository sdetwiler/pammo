#ifndef __NPCMANAGER_H__
#define __NPCMANAGER_H__

#include "pammo.h"
#include "player.h"
namespace pammo
{

class NpcManager
{
public:
    NpcManager();
    virtual ~NpcManager();

    int init(uint32_t npcCount);

    virtual void update();
    virtual void draw();

    Player* hitTest(Vector2 startPos, Vector2 endPos, float radius);

protected:

private:
    void generatePath(Player* player);

    Player* mPlayers;
    uint32_t mNumPlayers;

};


} // namespace pammo

#endif // __NPCMANAGER_H__