#ifndef __PAMMO_H__
#define __PAMMO_H__

#include "platform.h"

#include <vector>
#include <set>
#include <map>
#include <string>
using namespace std;

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "vector2.h"
#include "transform2.h"

namespace pammo
{

struct Image
{
    Vector2 mSize;
    uint32_t mTexture;
};

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

void dprintf(char const* format, ...);
Image* openImage(char const* filename);
Vector2 getFrameSize();
uint64_t getTime(void);

} // namespace pammo

using namespace pammo;

#include "game.h"

#endif // __PAMMO_H__