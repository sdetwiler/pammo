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
bool intersectLineAndCircle(Vector2 p1, Vector2 p2, float r, Vector2& hit, float& alpha)
{
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
    float farAlpha;
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
    
    // Choose "farHit" if it is closer to p1 or if hit is not in segment.
    if((alpha > farAlpha && farAlphaIn) || !alphaIn)
    {
        hit = farHit;
        alpha = farAlpha;
    }
    
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
    
    // Determine duration of time. Distance / speed.
    float length = sqrt((startPosition.x - endPosition.x)*(startPosition.x - endPosition.x) + (startPosition.y - endPosition.y)*(startPosition.y - endPosition.y));
    float eventTime =  length / speed;
    
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        switch((*i)->mType)
        {
        case DynamicBody::VehicleType:
        {
            Vehicle* vehicle = (*i)->mVehicle;

            if(!(mask & vehicle->getCollisionBodyMask()))
                continue;
            
            Vector2 vehicleMovement = Vector2(vehicle->getSpeed() * eventTime, 0) * Transform2::createRotation(vehicle->getDirection());

            Vector2 hit;
            float alpha;
            
            if(intersectLineAndCircle(startPosition - vehicle->mCenter, endPosition - vehicle->mCenter - vehicleMovement, vehicle->getCollisionBodyRadius() + radius, hit, alpha))
            {
                hit = startPosition + (endPosition - startPosition) * alpha;
                float dist = magnitude(startPosition - hit);
                //float dist = sqrt((startPosition.x - hit.x)*(startPosition.x - hit.x) + (startPosition.y - hit.y)*(startPosition.y - hit.y));
                
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

void CollisionDynamics::draw()
{
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(0, 0, 1, .25);
    
    for(DynamicBodyVector::iterator i = mBodies.begin(); i != mBodies.end(); ++i)
    {
        if((*i)->mType != DynamicBody::VehicleType) continue;
        
        Vehicle* vehicle = (*i)->mVehicle;
        uint32_t const num = 8;
        Vector2 points[num];
        for(uint32_t j=0; j < num; ++j)
            points[j] = vehicle->mCenter + Vector2(vehicle->getCollisionBodyRadius(), 0) * Transform2::createRotation(3.1415/4*j);
            
        glVertexPointer(2, GL_FLOAT, 0, (float*)points);
        glDrawArrays(GL_TRIANGLE_FAN, 0, num);
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

}
