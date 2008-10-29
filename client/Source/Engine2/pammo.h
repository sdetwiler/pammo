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


struct Audio; // Defined per platform in platform.h
struct AudioClip; // AudioLibrary.h
struct AudioInstance; // AudioLibrary.h

//int loadAudio(char const* filename, Audio* audio);

//#define AUDIO_VOLUME_MIN    0
//#define AUDIO_VOLUME_MAX    128

//int initializeAudio();
//void startAudio(Audio* audio, int volume=AUDIO_VOLUME_MAX);
//void stopAudio(Audio* audio);
//void updateAudio(Audio* audio);
//void unloadAudio(Audio* audio);


int openAudio_platform(AudioInstance* instance);
void closeAudio_platform(AudioInstance* instance);
void updateAudio_platform(AudioInstance* instance);
void stopAudio_platform(AudioInstance* instance);

void spawnThread(void* (*threadFunc)(void*), void* arg);

} // namespace pammo

using namespace pammo;

#include "game.h"

#endif // __PAMMO_H__