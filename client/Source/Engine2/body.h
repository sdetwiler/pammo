#ifndef __BODY_H__
#define __BODY_H__

#include "pammo.h"

namespace pammo
{

class Physics;

class Body
{
    public:
        Body();
        virtual ~Body();
        
        // Bit field describing this.
        uint16_t mProperties;
        
        // Bit field of what this collides with.
        // this->mProperties & other->mCollideProperties != 0 -> Collision.
        uint16_t mCollideProperties;
        
        Physics* mPhysics;
        void* mUserArg;
        
        float mMass;
        float mRadius;
        
        float mDamping;
        Vector2 mAcceleration;
        Vector2 mVelocity;
        Vector2 mCenter;
};

}

#endif