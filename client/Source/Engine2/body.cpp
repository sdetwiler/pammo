#include "body.h"

namespace pammo
{

Body::Body()
{
    mMass = 0;
    mRadius = 0;
    
    mDamping = 0;
    mAcceleration = Vector2(0, 0);
    mVelocity = Vector2(0, 0);
    mCenter = Vector2(0, 0);
}

Body::~Body()
{
}

}
