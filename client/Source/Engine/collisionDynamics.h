/*
 *  collisionDynamics.h
 *  pammo
 *
 *  Created by James Marr on 6/18/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __COLLISION_DYNAMICS_H__
#define __COLLISION_DYNAMICS_H__

#include "pammo.h"

namespace pammo
{

class Vehicle;

struct DynamicBody
{
    enum Type
    {
        VehicleType,
    };
    Type mType;
    Vehicle* mVehicle;
};

#define LOCALPLAYER    1
#define REMOTEPLAYER   2

class CollisionDynamics
{
    public:
        CollisionDynamics();
        ~CollisionDynamics();
        
        void addVehicle(Vehicle* vehicle);
        void removeVehicle(Vehicle* vehicle);
        
        struct RaycastResult
        {
            bool mHit;
            DynamicBody* mBody;
            Vector2 mPosition;
            float mDistance;
            uint32_t mTime;
        };
        
        void raycast(Vector2 const& startPosition, Vector2 const& endPosition, float radius, float speed, uint32_t mask, RaycastResult& result);
        Vehicle* getClosestVehicle(Vehicle const* fromVehicle);

        void draw();
        
    private:
        typedef vector< DynamicBody* > DynamicBodyVector;
        DynamicBodyVector mBodies;
};

}

#endif