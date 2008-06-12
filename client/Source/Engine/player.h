#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "pathManager.h"
#include "vehicle.h"

namespace pammo
{

class Player : 
    public Touchable,
    public Updateable
{
public:
    Player();
    virtual ~Player();

    int init();

    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getTouchPriority() const;
    virtual void update();

    bool isMoving();
    void draw();

    void setPath(Vector2Vec const& path);

    Vector2 const& getCenter() const;

protected:

private:
    PathManager* mPathManager;
    Vehicle*     mVehicle;
};

} // namespace pammo

#endif // __PLAYER_H__