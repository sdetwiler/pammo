#ifndef __VEHICLE_H__
#define __VEHICLE_H__

#include "pammo.h"
#include "entity.h"
#include "image.h"


namespace pammo
{
class World;

class Vehicle : 
    public Entity,
    public Touchable,
    public Updateable
{
public:
    Vehicle(World* world);
    virtual ~Vehicle();

    virtual int init();

    void setPath(Vector2Vec const& points);

    virtual void draw();
    virtual bool touch(uint32_t count, Touch* touches);
    virtual uint32_t getTouchPriority() const;
    virtual void update(int delta);


protected:
    World* mWorld;
    Image* mImage;
    float mSpeed; // in meters/second.
    bool mMoving;

private:
    Vector2Vec mPath;
    Vector2Vec::iterator mCurrTarget;
    Vector2 mPosition;

};

}

#endif // __VECHICLE_H__
