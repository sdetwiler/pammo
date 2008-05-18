/*
 *  world.h
 *  Flain
 *
 *  Created by James Marr on 3/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include "pammo.h"

namespace pammo
{

class Entity;
class Camera;
class GraphicsContext;

struct Touch
{
	enum Phase
	{
		TouchPhaseBegin,
		TouchPhaseMove,
		TouchPhaseEnd
	};
	
	Phase mPhase;
	Vector2 mLocation;
};

class World
{
	public:
		World(GraphicsContext* context);
		~World();
		
		void addEntity(Entity* entity);
		
		void draw();
		void update(int delta);
		
		void touches(uint32_t count, Touch* touches);
		
		GraphicsContext* mContext;
		Camera* mCamera;
		Vector2 mStartTouch;
		typedef vector<Entity*> EntityVector;
		EntityVector mEntities;
};

}

#endif
