/*
 *  Entity.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "entity.h"

Entity::Entity()
    : mRotation(0)
{
	mCenter.x = 0;
	mCenter.y = 0;
	
	mTransformDirty = true;
	mInverseTransformDirty = true;
    mDrawPriority = 0;
}

Entity::~Entity()
{
}

void Entity::setSize(Vector2 size)
{
    mSize = size;
}

void Entity::setDrawPriority(uint32_t prio)
{
    mDrawPriority = prio;
}

uint32_t Entity::getDrawPriority() const
{
    return mDrawPriority;
}

void Entity::makeDirty()
{
	mTransformDirty = true;
	mInverseTransformDirty = true;
}

Transform2 const& Entity::getTransform()
{
	if(mTransformDirty)
	{
		mTransform = Transform2();
		mTransform *= Transform2::createTranslation(mCenter);
		mTransform *= Transform2::createRotation(mRotation);
		mTransform *= Transform2::createTranslation(mSize * Vector2(-0.5, -0.5));
		mTransform *= Transform2::createScale(mSize);
		
		mTransformDirty = false;
	}
	
	return mTransform;
}

Transform2 const& Entity::getInverseTransform()
{
	if(mInverseTransformDirty)
	{
		//CGAffineTransform tmp = CGAffineTransformInvert(CGAffineTransformMake(
		//	mTransform[0], mTransform[1],
		//	mTransform[2], mTransform[3],
		//	mTransform[4], mTransform[5]));
		//mInverseTransform[0] = tmp.a;
		//mInverseTransform[1] = tmp.b;
		//mInverseTransform[2] = tmp.c;
		//mInverseTransform[3] = tmp.d;
		//mInverseTransform[4] = tmp.tx;
		//mInverseTransform[5] = tmp.ty;
		
		//mInverseTransform = CGAffineTransformInvert(getTransform());
		
		mInverseTransform = inverse(getTransform());
		mInverseTransformDirty = false;
	}
	
	return mInverseTransform;
}
		
