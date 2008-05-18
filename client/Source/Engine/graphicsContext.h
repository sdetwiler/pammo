/*
 *  graphicsContext.h
 *  Flain
 *
 *  Created by James Marr on 4/1/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __GRAPHICSCONTEXT_H__
#define __GRAPHICSCONTEXT_H__

#include "pammo.h"

namespace pammo
{

class Image
{
	public:
		virtual Vector2 getSize() = 0;
};

class GraphicsContext
{
	public:
		virtual ~GraphicsContext() {};
		
		virtual Vector2 getFrameSize() = 0;
		
		virtual void frameBegin() = 0;
		virtual void frameEnd() = 0;
		
		virtual void setCameraTransform(Transform2 const& transform) = 0;
		
		virtual Image* openImage(char const* path) = 0;
		virtual void closeImage(Image* image) = 0;
		virtual void drawImage(Image* image, Transform2 const& transform, Transform2 const& invTransform) = 0;
};

}

#endif