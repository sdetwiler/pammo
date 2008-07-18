#ifndef __ENTITY_H__
#define __ENTITY_H__
 
#include "pammo.h"

namespace pammo
{

class Entity
{
    public:
		Entity();
		virtual ~Entity();
		
		virtual void draw()=0;
		void setSize(Vector2 size);

		void makeDirty();
		
		Vector2 mCenter;
		Vector2 mSize;
		float mRotation;
        void setTransform(Transform2 const& trans);
		Transform2 const& getTransform();
		
	protected:
		Transform2 const& getInverseTransform();

    private:	
		Transform2 mTransform;
		bool mTransformDirty;
		Transform2 mInverseTransform;
		bool mInverseTransformDirty;

};

}

#endif