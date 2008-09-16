// Trick to get fast precompiled headers in xCode. Don't touch.
#ifdef pammo
#undef pammo
#endif

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

struct RawImage
{
    Vector2 mSize;
    uint8_t* mPixels;
    uint8_t mBytesPerPixel;
};

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
    void* mSerialNumber;
};

void dprintf(char const* format, ...);
void openRawImage(char const* filename, RawImage* image);
Image* makeSubImage(RawImage* raw, Vector2 start, Vector2 size);
Image* openImage(char const* filename);
Vector2 getFrameSize();
uint64_t getTime(void);

} // namespace pammo

using namespace pammo;

#include "game.h"

#endif // __PAMMO_H__