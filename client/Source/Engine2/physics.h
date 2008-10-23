#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "pammo.h"
#include "view.h"

namespace pammo
{

enum CollisionProperties
{
    kPlayerCollisionProperties = 1 << 0,
    kPlayerBulletCollisionProperties = 1 << 1,
    kPlayerBarrierCollisionProperties = 1 << 2,
    kEnemyCollisionProperties = 1 << 3,
    kEnemyBulletCollisionProperties = 1 << 4,
    kEnemyBarrierCollisionProperties = 1 << 5,
    kPowerupCollisionProperties = 1<<6,
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
typedef void (*BodyOnShapeCollisionCallback)(Body* self, Shape* other, Contact* contact, bool* response);

struct Shape
{
    // Bit field describing this.
    uint16_t mProperties;
    
    // Bounds of shape.
    uint16_t mNumPoints;
    Vector2* mPoints;
    
    // Private, for list management.
    uint64_t mLastQuery;
    Shape* mNext;
};

struct ShapeBucket
{
    Shape* mShape;
    ShapeBucket* mNext;
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

const uint32_t kShapeBucketCount = 30;

class Physics : public View
{
    public:
        Physics(uint32_t numBodies);
        ~Physics();
    
        virtual uint32_t getUpdatePriority() const;
        virtual uint32_t getDrawPriority() const;
    
        virtual void update();
        virtual void draw();
        
        void reset();

        void setMapSize(Vector2 mapBounds);
        void addShape(uint16_t properties, uint16_t numPoints, Vector2* points);
        
        Body* addBody();
        void removeBody(Body* body);
        void resortBody(Body* body);
        
    private:
        Body* mBodies;
        Body* mEdges;
        Body* mAddBodies;
        Body* mRemoveBodies;
        Body* mFreed;

        Shape* mShapes;
        Vector2 mShapeBucketSize;
        uint64_t mShapeQuery;
        ShapeBucket* mShapeBuckets[kShapeBucketCount][kShapeBucketCount];
        ShapeBucket* mShapeBucketOffscreen;
        
        void integrate();
        void collide();
        
        void collideAgainstBodies(Body* b1);
        void collideAgainstShapes(Body* b1);
        void collideAgainstBucket(Body* b1, ShapeBucket* bucket, float radiusSquared);
        
        void oneBodyResponse(Body* b1, Contact* contact);
        void twoBodyResponse(Body* b1, Body* b2, Contact* contact);
};

}

#endif
