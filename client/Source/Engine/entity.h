/*
 *  entity.h
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */ 
 
#ifndef __ENTITY_H__
#define __ENTITY_H__
 
#include "pammo.h"
#include "image.h"

namespace pammo
{

class Entity
{
	public:
		Entity(Image* image);
		virtual ~Entity();
		
		virtual void draw();
		
		void makeDirty();
		
		Image* mImage;
		Vector2 mCenter;
		Vector2 mSize;
		float mRotation;
		
	private:
		Transform2 const& getTransform();
		Transform2 const& getInverseTransform();
		
		Transform2 mTransform;
		bool mTransformDirty;
		Transform2 mInverseTransform;
		bool mInverseTransformDirty;
};

typedef vector<Entity*> EntityVector;

}

#endif