#ifndef __PATHMANAGER_H__
#define __PATHMANAGER_H__

#include "pammo.h"
#include "game.h"

namespace pammo
{
class Player;

class PathManager : 
    public Initable,
    public Deleteable,
    public Drawable,
    public Touchable
{
public:
    PathManager(Player* player);
    virtual ~PathManager();

    void destroy();
    virtual void init();
    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getDrawPriority() const;
    virtual uint32_t getTouchPriority() const;

protected:
    void addPoint(Vector2 const& point);
    void clear();

private:
    Player* mPlayer;
    Vector2Vec mPoints;
    bool mBuilding;
};

}

#endif __PATHMANAGER_H__
