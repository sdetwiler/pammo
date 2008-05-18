/*
 *  Entity.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "entity.h"
#include "graphicsContext.h"

Entity::Entity(Image* image)
	: mImage(image),
	mRotation(0)
{
	mCenter.x = 0;
	mCenter.y = 0;
	
	mSize = image->getSize();
	
	mTransformDirty = true;
	mInverseTransformDirty = true;
}

Entity::~Entity()
{
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
		
void Entity::draw(GraphicsContext* context)
{
	context->drawImage(mImage, getTransform(), getInverseTransform());
	
	//Transform2 tmp;
	//CGAffineTransform cgtmp;
	
	//tmp = getTransform();
	//cgtmp = CGAffineTransformMake(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
	//CGContextConcatCTM(context, cgtmp);
	
	//CGContextConcatCTM(context, getTransform());
	
	//CGRect rect;
	//rect.origin.x = 0;
	//rect.origin.y = 0;
	//rect.size.width = 1;
	//rect.size.height = 1;
	
	//CGContextDrawImage(context, rect, mImage);
	
	//tmp = getInverseTransform();
	//cgtmp = CGAffineTransformMake(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
	//CGContextConcatCTM(context, cgtmp);
	//CGContextConcatCTM(context, getInverseTransform());
}