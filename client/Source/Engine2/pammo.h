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
#include <pthread.h>

#include "vector2.h"
#include "transform2.h"

namespace pammo
{

struct RawImage
{
    Vector2 mSize;
    Vector2 mPixelSize;
    uint8_t* mPixels;
    uint8_t mBytesPerPixel;
};

struct Image
{
    Vector2 mSize;
    Vector2 mTexCoords[4];
    uint32_t mTexture;
    uint32_t mTableIndex; // To support reference/dereference images from Image*
    int      mMode; // GL_RGB, GL_RGBA
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
Vector2 getFrameSize();
uint64_t getTime(void);

void spawnThread(void* (*threadFunc)(void*), void* arg);

} // namespace pammo

using namespace pammo;

#include "game.h"

#endif // __PAMMO_H__