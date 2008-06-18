/*
 *  collisionDynamics.cpp
 *  pammo
 *
 *  Created by James Marr on 6/18/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "collisionDynamics.h"

#include "vehicle.h"

namespace pammo
{

// Intersect a line segment with a circle
bool intersectLineAndCircle(Vector2 p1t, Vector2 p2t, Vector2 c, float r, Vector2& hit, float& dist, Vector2& farHit)
{
    // First itneresect a line segment with a sphere.
    // http://mathworld.wolfram.com/Circle-LineIntersection.html
    
    Vector2 p1 = p1t - c;
    Vector2 p2 = p2t - c;
    
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dr = sqrt(dx*dx + dy*dy);
    float D = p1.x*p2.y - p2.x*p1.y;
    
    float delta = r*r*dr*dr - D*D;
    
    if(delta <= 0)
        return false;
    
    float sqrtdelta = sqrt(delta);
    float sgndy = 1;
    if(dy < 0) sgndy = -1;
    
    hit.x = (D*dy + sgndy*dx*sqrtdelta)/(dr*dr) + c.x;
    hit.y = (-D*dx + fabs(dy)*sqrtdelta)/(dr*dr) + c.y;
    
    farHit.x = (D*dy - sgndy*dx*sqrtdelta)/(dr*dr) + c.x;
    farHit.y = (-D*dx - fabs(dy)*sqrtdelta)/(dr*dr) + c.y;
    
    dist = sqrt((hit.x - p1t.x)*(hit.x - p1t.x) + (hit.y - p1t.y)*(hit.y - p1t.y));
    
    return true;
}

CollisionDynamics::CollisionDynamics()
{
}

CollisionDynamics::~CollisionDynamics()
{
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
        delete *i;
}

void CollisionDynamics::addVehicle(Vehicle* vehicle)
{
    DynamicBody* body = new DynamicBody();
    body->mType = DynamicBody::VehicleType;
    body->mVehicle = vehicle;
    mBodies.push_back(body);
}

void CollisionDynamics::removeVehicle(Vehicle* vehicle)
{
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        if((*i)->mType == DynamicBody::VehicleType && (*i)->mVehicle == vehicle)
        {
            delete *i;
            mBodies.erase(i);
            return;
        }
    }
    assert(0);
}

void CollisionDynamics::raycast(Vector2 const& startPosition, Vector2 const& endPosition, float radius, float speed, uint32_t mask, RaycastResult& result)
{
    result.mHit = false;
    return;
    
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        switch((*i)->mType)
        {
        case DynamicBody::VehicleType:
        {
            Vehicle* vehicle = (*i)->mVehicle;
            
            Vector2 hit, farHit;
            float dist;
            
            if(intersectLineAndCircle(startPosition, endPosition, vehicle->mCenter, vehicle->getCollisionBodyRadius(), hit, dist, farHit))
            {
                if(!result.mHit || dist < result.mDistance)
                {
                    result.mHit = true;
                    result.mBody = *i;
                    result.mTime = 0;
                    result.mDistance = dist;
                    result.mPosition = hit;
                    result.mFarPosition = farHit;
                }
            }
            break;
        }
        default:
            assert(0);
        }
    }
}

}
