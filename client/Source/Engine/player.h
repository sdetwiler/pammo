#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "pathManager.h"
#include "vehicle.h"

namespace pammo
{
class Player;

class PlayerObserver
{
public:
    virtual ~PlayerObserver(){}
    virtual void onHealthChanged(Player* player, float health)=0;
};

class Player : 
    public Touchable,
    public Updateable,
    public VehicleObserver
{
public:
    enum Type
    {
        Local,
        Remote
    };

    enum State
    {
        Spawning,
        Alive,
        Destroyed
    };


    Player();
    virtual ~Player();

    int init(Player::Type type);

    void setObserver(PlayerObserver* o);

    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getTouchPriority() const;
    virtual void update();

    virtual void onHit(Vehicle* vehicle, float damage);

    bool isMoving();
    void draw();

    void setPath(Vector2Vec const& path);
    void setHealth(float h);
    float getHealth() const;

    State getState() const;
    void setState(State state);

    Vector2 const& getCenter() const;


protected:

    void spawn();
    void destroy();

private:
    PathManager* mPathManager;
    Vehicle*     mVehicle;
    Type         mType;
    State        mState;

    uint64_t     mSpawnFrame;  // Frame number was spawned.
    uint64_t     mFrameCount;  // Count of frames since created.

    float        mHealth;

    PlayerObserver* mObserver;
};

} // namespace pammo

#endif // __PLAYER_H__