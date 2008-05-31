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
#include "game.h"

namespace pammo
{

class Entity :
    public Drawable
{
    public:
		Entity();
		virtual ~Entity();
		
		virtual void draw()=0;
        virtual uint32_t getDrawPriority() const;

        void setDrawPriority(uint32_t prio);
		void setSize(Vector2 size);

		void makeDirty();
		
		Vector2 mCenter;
		Vector2 mSize;
		float mRotation;
		Transform2 const& getTransform();
		
	protected:
		Transform2 const& getInverseTransform();

    private:	
		Transform2 mTransform;
		bool mTransformDirty;
		Transform2 mInverseTransform;
		bool mInverseTransformDirty;
        uint32_t mDrawPriority;

};

}

#endif