#ifndef __PAMMO_H__
#define __PAMMO_H__

#include "platform.h"
#include "dprintf.h"

#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

#include <stdlib.h>
#include <math.h>

#include "vector2.h"
#include "transform2.h"

namespace pammo
{
struct Touch
{
	enum Phase
	{
		PhaseBegin,
		PhaseMove,
		PhaseEnd
	};
	
	Phase mPhase;
	Vector2 mLocation;
};

typedef std::vector< Vector2 > Vector2Vec;

} // namespace pammo

using namespace pammo;

#endif // __PAMMO_H__