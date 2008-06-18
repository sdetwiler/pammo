/*
 *  collisionMap.h
 *  pammo
 *
 *  Created by James Marr on 6/9/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __COLLISION_MAP_H__
#define __COLLISION_MAP_H__

#include "pammo.h"

namespace pammo
{

class CollisionMap
{
public:
    CollisionMap();
    ~CollisionMap();
    
    void addShape(uint16_t numPoints, Vector2* points);
    
    struct Shape
    {
        uint16_t mNumPoints;
        Vector2* mPoints;
    };
    
    struct RaycastResult
    {
        bool mHit;
        Vector2 mPosition;
        Vector2 mFarPosition;
        float mDistance;
        Shape* mShape;
        uint16_t mFace;
    };
    
    void raycast(Vector2 startPos, Vector2 endPos, float radius, RaycastResult& result);
    bool route(Vector2 startPos, Vector2 endPos, float radius, vector< Vector2 >& newPath);
    void draw();
    
public:
    typedef vector< Shape* > ShapeVector;
    ShapeVector mShapes;
};

}

#endif