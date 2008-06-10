/*
 *  collisionMap.cpp
 *  pammo
 *
 *  Created by James Marr on 6/9/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "collisionMap.h"

namespace pammo
{

CollisionMap::CollisionMap()
{
}

CollisionMap::~CollisionMap()
{
    for(ShapeVector::iterator i=mShapes.begin(); i != mShapes.end(); ++i)
    {
        delete[] (*i)->mPoints;
        delete *i;
    }
}
    
void CollisionMap::addShape(uint16_t numPoints, Vector2* points)
{
    Shape* s = new Shape();
    s->mNumPoints = numPoints;
    s->mPoints = new Vector2[numPoints];
    
    for(uint16_t i=0; i < numPoints; ++i)
    {
        s->mPoints[i] = points[i];
    }
    mShapes.push_back(s);
}
    
bool CollisionMap::intersect(Vector2 startPos, Vector2 endPos, float radius, Vector2& hit)
{
    return false;
}

void CollisionMap::draw()
{
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(0, 1, 0, .25);
    
    for(ShapeVector::iterator shape = mShapes.begin(); shape != mShapes.end(); ++shape)
    {
        glVertexPointer(2, GL_FLOAT, 0, (float*)(*shape)->mPoints);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (*shape)->mNumPoints);
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
    
}