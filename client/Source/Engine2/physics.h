#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

enum CollisionProperties
{
    kPlayerCollisionProperties = 1 << 0,
    kEnemyCollisionProperties = 1 << 1,
    kPlayerBulletCollisionProperties = 1 << 2
};

class Physics;
struct Body;
struct Shape;

// Description of a contact event.
struct Contact
{
    Vector2 mContactPoint;
    Vector2 mContactNormal;
    float mSeparatingVelocity;
    float mPenetrationDepth;
};

// Responses to a contact.
struct ContactResponse
{
    bool mBounceMe;
    bool mBounceThem;
};

// Callback function.
typedef void (*BodyOnBodyCollisionCallback)(Body* self, Body* other, Contact* contact, ContactResponse* response);
typedef void (*BodyOnShapeCollisionCallback)(Body* self, Shape* other, Contact* contact, ContactResponse* response);

struct Shape
{
    // Bit field describing this.
    uint16_t mProperties;
    
    // Bounds of shape.
    uint16_t mNumPoints;
    Vector2* mPoints;
};

struct Body
{
    // Bit field describing this.
    uint16_t mProperties;
    
    // Bit field of what this collides with.
    // this->mProperties & other->mCollideProperties != 0 -> Collision.
    uint16_t mCollideProperties;
    
    // Pointers back to objects.
    void* mUserArg;
    
    // Static body attributes.
    float mMass;
    float mRadius;
    float mDamping;
    
    // Dynamic body attributes.
    Vector2 mAcceleration;
    Vector2 mVelocity;
    Vector2 mCenter;
    
    // Collision callbacks.
    BodyOnBodyCollisionCallback mBodyCallback;
    BodyOnShapeCollisionCallback mShapeCallback;
    
    // List maitenance.
    bool mAdded;
    Body* mNext;
    Body* mPrev;
    Body* mEdgeNext;
    Body* mEdgePrev;
    Body* mRemoveNext;
};

class Physics : public View
{
    public:
        Physics();
        ~Physics();
    
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
    
        virtual void update();
        virtual void draw();
        
        Shape* addShape();
        
        Body* addBody();
        void removeBody(Body* body);
        void resortBody(Body* body);
        
    private:
        Body* mBodies;
        Body* mEdges;
        Body* mAddBodies;
        Body* mRemoveBodies;
        Body* mFreed;
        
        void integrate();
        void collide();
};

}

#endif
