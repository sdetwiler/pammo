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
    
    bool intersect(Vector2 startPos, Vector2 endPos, float radius, Vector2& hit);
    void draw();
    
public:
    struct Shape
    {
        uint16_t mNumPoints;
        Vector2* mPoints;
    };
    typedef vector< Shape* > ShapeVector;
    ShapeVector mShapes;
};

}

#endif