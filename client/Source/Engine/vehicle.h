#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "pammo.h"
#include "entity.h"
#include "animationEntity.h"

namespace pammo
{
class World;
class Vehicle;

class VehicleObserver
{
public:
    virtual ~VehicleObserver(){}
    virtual void onHit(Vehicle* vehicle, float damage) = 0;
};

class Vehicle : 
    public Entity,
    public Touchable,
    public Updateable
{
public:
    Vehicle();
    virtual ~Vehicle();

    virtual int init();

    void setPath(Vector2Vec const& points);

    virtual void draw();

    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getTouchPriority() const;
    virtual void update();

    // Called when vehicle is hit to cause damage.
    virtual void hit(float damage)=0;
    
    // Destroys the vehicle... in the ka-blooie! sense, not the memory, garbage collection sense.
    virtual void destroy() = 0;

    void setObserver(VehicleObserver* o);
    VehicleObserver* getObserver();

    bool isMoving();

    void setCollisionBodyMask(uint32_t mask);
    
    uint32_t getCollisionBodyMask() const;
    float getCollisionBodyRadius() const; 
    float getSpeed() const;
    float getDirection() const;

protected:
    World* mWorld;
    AnimationEntity mAnimation;
    float mSpeed; // in meters/second.
    bool mMoving;

    float mOldTheta;
    
    uint32_t mCollisionBodyMask;
    float mCollisionBodyRadius;

private:
    Vector2Vec mPath;
    Vector2Vec::iterator mCurrTarget;
    Vector2 mPosition;
    VehicleObserver* mObserver;
};

}

#endif // __VECHICLE_H__
