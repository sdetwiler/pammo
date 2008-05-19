/*
 *  camera.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "camera.h"
#include "graphicsContext.h"

namespace pammo
{

Camera::Camera(Vector2 const& origin, Vector2 const& size)
{
	mCenter.x = 0;
	mCenter.y = 0;
	
	mSize.x = 320;
	mSize.y = 480;
	
	mRotation = 0;//3.14159/4;
	
	mFrameStart = origin;
	mFrameSize = size;
	
	mTransformDirty = true;
	mInverseTransformDirty = true;
}

Camera::~Camera()
{}

void Camera::makeDirty()
{
	mTransformDirty = true;
	mInverseTransformDirty = true;
}

Transform2 Camera::getTransform()
{
	if(mTransformDirty)
	{
		//mTransform = CGAffineTransformIdentity;
		//mTransform = CGAffineTransformTranslate(mTransform,  mFrameSize.x/2, mFrameSize.y/2);
		//mTransform = CGAffineTransformRotate(mTransform, mRotation);
		//mTransform = CGAffineTransformScale(mTransform, mFrameSize.x / mSize.x, mFrameSize.y / mSize.y);
		//mTransform = CGAffineTransformTranslate(mTransform, - mCenter.x, - mCenter.y);
		
		mTransform = Transform2();
		mTransform *= Transform2::createTranslation(Vector2(mFrameSize.x/2, mFrameSize.y/2));
		mTransform *= Transform2::createRotation(mRotation);
		mTransform *= Transform2::createScale(Vector2(mFrameSize.x / mSize.x, mFrameSize.y / mSize.y));
		mTransform *= Transform2::createTranslation(Vector2(-mCenter.x, -mCenter.y));
		
		dprintf("Frame: %f, %f\n", mFrameSize.x, mFrameSize.y);
		dprintf("Rotation: %f\n", mRotation);
		dprintf("Center: %f, %f\n", mCenter.x, mCenter.y);
		
		dprintf("Camera Transform\n");
		dprintf("[ %f %f ]\n", mTransform[0], mTransform[1]);
		dprintf("[ %f %f ]\n", mTransform[2], mTransform[3]);
		dprintf("[ %f %f ]\n", mTransform[4], mTransform[5]);
		
		mTransformDirty = false;
	}
	
	return mTransform;
}

Transform2 Camera::getInverseTransform()
{
	if(mInverseTransformDirty)
	{
		mInverseTransform = inverse(getTransform());
		mInverseTransformDirty = false;
	}
	
	return mInverseTransform;
}
		
void Camera::set()
{
    getTransform();
    glLoadIdentity();
    float trans[16];
    memset(trans, 0, sizeof(trans));
    trans[0] = mTransform[0];
    trans[1] = mTransform[1];
    trans[4] = mTransform[2];
    trans[5] = mTransform[3];
    trans[10] = 1.0;
    trans[12] = mTransform[4];
    trans[13] = mTransform[5];
    trans[15] = 1.0;

    glMultMatrixf(trans);
	
	//CGContextConcatCTM(context, getTransform());
	//CGContextTranslateCTM(context, -mCenter.x + mScale.width/2, -mCenter.y + mScale.height/2);
	//CGContextRotateCTM(context, mRotation);
	//CGContextTranslateCTM(context, -mScale.width/2, -mScale.height/2);
	//CGContextScaleCTM(context, 320 / mScale.width, 480 / mScale.height);
	
	//dprintf("Set camera information");
}

Vector2 Camera::translateToScreen(Vector2 const& worldSpace)
{
	return worldSpace * getTransform();
	//return CGPointApplyAffineTransform(worldSpace, getTransform());
}

Vector2 Camera::translateToWorld(Vector2 const& screenSpace)
{
	return screenSpace * getInverseTransform();
	//return CGPointApplyAffineTransform(screenSpace, getInverseTransform());
}

}
