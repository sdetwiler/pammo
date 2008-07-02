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

// Intersect two line segments, formed by p1->p2 and p3->p4.
bool intersectLineAndLine(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2& hit, float& dist, Vector2& farHit)
{
    // First itneresect the two lines, ignoring endpoints.
    // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
    
    float det = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
    if(det == 0) return false;
    
    //float len = sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
    float len = magnitude(p2 - p1);
    float alpha = ((p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x))/det;
    float beta = ((p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x))/det;
    
    const float offset = 0.01;
    
    if(alpha < 0 || alpha > 1 || beta < 0 || beta > 1)
        return false;
    
    alpha -= offset;
    
    dist = alpha * len;
    hit.x = p1.x + alpha*(p2.x - p1.x);
    hit.y = p1.y + alpha*(p2.y - p1.y);
    
    alpha += offset*2;
    farHit.x = p1.x + alpha*(p2.x - p1.x);
    farHit.y = p1.y + alpha*(p2.y - p1.y);
    return true;
}

CollisionMap::CollisionMap()
{}

CollisionMap::~CollisionMap()
{
    for(ShapeVector::iterator i=mShapes.begin(); i != mShapes.end(); ++i)
    {
        delete[] (*i)->mPoints;
        delete[] (*i)->mNormals;
        delete *i;
    }
}
    
void CollisionMap::addShape(uint16_t numPoints, Vector2* points, Vector2* normals)
{
    Shape* s = new Shape();
    s->mNumPoints = numPoints;
    s->mPoints = new Vector2[numPoints];
    s->mNormals = new Vector2[numPoints];
    
    for(uint16_t i=0; i < numPoints; ++i)
    {
        s->mPoints[i] = points[i];
        s->mNormals[i] = normals[i];
    }
    mShapes.push_back(s);
}

bool CollisionMap::route(Vector2 startPos, Vector2 endPos, float radius, vector< Vector2 >& newPath)
{
    Vector2 hit;
    newPath.push_back(startPos);
    
    RaycastResult result;
    raycast(startPos, endPos, radius, result);
    if(!result.mHit)
    {
        newPath.push_back(endPos);
        return true;
    }
    
    RaycastResult result2;
    raycast(result.mFarPosition, endPos, radius, result2);
    if(!result2.mHit || result.mShape != result2.mShape)
    {
        newPath.push_back(result.mPosition);
        return true;
    }
    
    uint32_t curFace = result.mFace;
    while(curFace != result2.mFace)
    {
        newPath.push_back(result.mShape->mPoints[curFace] + result.mShape->mNormals[curFace]*radius);
        if(curFace == 0)
            curFace = result.mShape->mNumPoints-1;
        else
            curFace -= 1;
    }
    
    newPath.push_back(endPos);
        
    return true;
}
    
void CollisionMap::raycast(Vector2 startPos, Vector2 endPos, float radius, RaycastResult& result)
{
    result.mHit = false;
    
    for(ShapeVector::iterator shape = mShapes.begin(); shape != mShapes.end(); ++shape)
    {
        for(uint32_t i=0; i < (*shape)->mNumPoints; ++i)
        {
            float curDist;
            Vector2 curHit, curFarHit;
            
            uint32_t iPlusOne = (i+1) % (*shape)->mNumPoints;
            
            if(!intersectLineAndLine(startPos, endPos,
                (*shape)->mPoints[i] + (*shape)->mNormals[i]*radius,
                (*shape)->mPoints[iPlusOne] + (*shape)->mNormals[iPlusOne]*radius,
                curHit, curDist, curFarHit))
                continue;
                
            if(!result.mHit || curDist < result.mDistance)
            {
                result.mHit = true;
                
                result.mDistance = curDist;
                result.mPosition = curHit;
                result.mFarPosition = curFarHit;
                result.mShape = *shape;
                result.mFace = i;
            }
        }
    }
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
        
        //continue;
        
        // Draw expanded shape.
        glColor4f(0, 0.5, 0.5, .25);
        Vector2* points = new Vector2[(*shape)->mNumPoints];
        
        for(uint32_t i=0; i < (*shape)->mNumPoints; ++i)
            points[i] = (*shape)->mPoints[i] + (*shape)->mNormals[i]*16;
        glVertexPointer(2, GL_FLOAT, 0, (float*)points);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (*shape)->mNumPoints);
        delete[] points;
        glColor4f(0, 1, 0, .25);
    }
    
    glColor4f(1, 1, 1, 1);
    glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
    
}