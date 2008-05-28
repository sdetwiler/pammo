/*
 *  camera.h
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "pammo.h"

namespace pammo
{

class GraphicsContext;

class Camera
{
	public:
		Camera(Vector2 const& origin, Vector2 const& size);
		~Camera();
		
		void set();
	    void unset();
		
		void makeDirty();
		
		Vector2 translateToScreen(Vector2 const& worldSpace);
		Vector2 translateToWorld(Vector2 const& screenSpace);
		
		Vector2 mCenter;
		Vector2 mSize;
		float mRotation;
		Vector2 mFrameStart;
		Vector2 mFrameSize;
		
	private:
		Transform2 getTransform();
		Transform2 getInverseTransform();
		
		Transform2 mTransform;
		Transform2 mInverseTransform;
		bool mTransformDirty;
		bool mInverseTransformDirty;
};

}

#endif