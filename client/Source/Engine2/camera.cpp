/*
 *  camera.cpp
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "camera.h"

namespace pammo
{

Camera::Camera()
{
	mCenter = Vector2(0, 0);
	
	mSize = getFrameSize();
	
	mRotation = 0;//3.14159/4;
	
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
		mTransform *= Transform2::createTranslation(getFrameSize()/2.);
		mTransform *= Transform2::createRotation(mRotation);
		mTransform *= Transform2::createScale(getFrameSize() / mSize);
		mTransform *= Transform2::createTranslation(-mCenter);
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
		
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glMultMatrixf(trans);
	glMatrixMode(GL_MODELVIEW);
}
	
void Camera::unset()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
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
