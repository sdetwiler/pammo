/*
 *  aspenGraphicsContext.h
 *  Pammo
 *
 *  Created by James Marr on 4/20/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#ifndef __ASPENGRAPHICSCONTEXT_H__
#define __ASPENGRAPHICSCONTEXT_H__

#include "pammo.h"
#include "graphicsContext.h"

#include <CoreGraphics/CGContext.h>

namespace pammo
{

class AspenImage
	: public Image
{
	public:
		virtual Vector2 getSize();
		
		Vector2 mSize;
		CGImage* mImage;
};

class AspenGraphicsContext
	: public GraphicsContext
{
	public:
		AspenGraphicsContext();
		virtual ~AspenGraphicsContext();
		
		virtual Vector2 getFrameSize();
		
		virtual void frameBegin();
		virtual void frameEnd();
		
		virtual void setCameraTransform(Transform2 const& transform);
		
		virtual Image* openImage(char const* path);
		virtual void closeIage(Image* image);
		virtual void drawImage(Image* image, Transform2 const& transform, Transform2 const& invTransform);
		
		CGContext* mContext;

};

}

#endif