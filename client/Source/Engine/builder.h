/*
 *  builder.h
 *  Flain
 *
 *  Created by James Marr on 4/1/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef __BUILDER_H__
#define __BUILDER_H__
 
#include "pammo.h"

namespace pammo
{

class World;

void buildFromMap(World* world, char const* mapName);

}

#endif
