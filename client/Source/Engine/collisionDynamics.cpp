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
bool intersectLineAndCircle(Vector2 p1t, Vector2 p2t, Vector2 c, float r, Vector2& hit, float& dist)
{
    // Offset p1t and p2t into collision space. This assumes that the circle is at the origin.
    Vector2 p1 = p1t - c;
    Vector2 p2 = p2t - c;
    
    // First itneresect a line segment with a sphere.
    // http://mathworld.wolfram.com/Circle-LineIntersection.html
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dr = sqrt(dx*dx + dy*dy);
    float D = p1.x*p2.y - p2.x*p1.y;
    
    float delta = r*r*dr*dr - D*D;
    if(delta <= 0) return false;
    
    float sqrtdelta = sqrt(delta);
    float sgndy = 1;
    if(dy < 0) sgndy = -1;
    
    hit.x = (D*dy + sgndy*dx*sqrtdelta)/(dr*dr);
    hit.y = (-D*dx + fabs(dy)*sqrtdelta)/(dr*dr);
    
    Vector2 farHit;
    farHit.x = (D*dy - sgndy*dx*sqrtdelta)/(dr*dr);
    farHit.y = (-D*dx - fabs(dy)*sqrtdelta)/(dr*dr);
    // End dense-math reference section.
    
    // Calculate how far along the ray both hit and farHit are.
    float alpha, farAlpha;
    if(p2.x - p1.x != 0)
    {
        alpha = (hit.x - p1.x)/(p2.x - p1.x);
        farAlpha = (farHit.x - p1.x)/(p2.x - p1.x);
    }
    else
    {
        alpha = (hit.y - p1.y)/(p2.y - p1.y);
        farAlpha = (farHit.y - p1.y)/(p2.y - p1.y);
    }
    
    // Calculate whether hit and farHit are within the segment.
    bool alphaIn, farAlphaIn;
    alphaIn = alpha >= 0 && alpha <= 1;
    farAlphaIn = farAlpha >= 0 & farAlpha <= 1;
    
    // If both out, this is not a hit.
    if(!alphaIn && !farAlphaIn)
        return false;
        
    // Calculate the distances from p1 to hit and farHit.
    dist = sqrt((hit.x - p1t.x)*(hit.x - p1t.x) + (hit.y - p1t.y)*(hit.y - p1t.y));
    float farDist = sqrt((farHit.x - p1t.x)*(farHit.x - p1t.x) + (farHit.y - p1t.y)*(farHit.y - p1t.y));
    
    // Choose "farHit" if it is closer to p1 or if hit is not in segment.
    if((dist > farDist && farAlphaIn) || !alphaIn)
    {
        hit = farHit;
        dist = farDist;
    }
    
    // Offset hit back into non-collision space.
    hit += c;
    
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
}

void CollisionDynamics::raycast(Vector2 const& startPosition, Vector2 const& endPosition, float radius, float speed, uint32_t mask, RaycastResult& result)
{
    result.mHit = false;
    //return;
    
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        switch((*i)->mType)
        {
        case DynamicBody::VehicleType:
        {
            Vehicle* vehicle = (*i)->mVehicle;

            if(!(mask & vehicle->getCollisionBodyMask()))
                continue;

            Vector2 hit;
            float dist;
            
            if(intersectLineAndCircle(startPosition, endPosition, vehicle->mCenter, vehicle->getCollisionBodyRadius(), hit, dist))
            {
                if(!result.mHit || dist < result.mDistance)
                {
                    result.mHit = true;
                    result.mBody = *i;
                    result.mTime = 0;
                    result.mDistance = dist;
                    result.mPosition = hit;
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
