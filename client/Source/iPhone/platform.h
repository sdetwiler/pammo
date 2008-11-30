#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import <AudioToolbox/ExtendedAudioFile.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>


#include <dirent.h>
#include <stdint.h>
#include <sys/time.h>

#define NUM_AUDIO_BUFFERS 2

namespace pammo
{

struct Audio	
{
    AudioFileID mFile;
    UInt32 mMaxPacketSize;    // Maximum packet size, in bytes.
    UInt64 mNumPackets;       // Total packet count for file.
	UInt32 mCurrentPacket;    // Current packet in stream.

    UInt64 mCurrentByte;
    
    UInt32 mReadFreq;
    UInt64 mNextRead;

    AudioStreamBasicDescription mFormat;
};

    
    
    
} // namespace pammo

	
//#define PROFILE

#define PAMMO_GL_16BIT GL_UNSIGNED_SHORT_5_6_5
#define PAMMO_GL_8BIT_RGA GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define PAMMO_GL_8BIT_RGBA GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG

#define PAMMO_MAP_PREVIEW_PATH "backdroppvrtc/paris/preview.pvrtc"
#define PAMMO_MAP_TILES_FORMAT "data/backdroppvrtc/paris/%d-%d.pvrtc"

#endif // __PLATFORM_H__