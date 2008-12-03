#include "pammo.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <QuartzCore/QuartzCore.h>
#include <CoreFoundation/CFURL.h>

#import <AudioToolbox/ExtendedAudioFile.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>

#include "audioLibrary.h"

//#include <OpenAL/al.h>
//#include <OpenAL/alc.h>
//#include <OpenAL/oalStaticBufferExtension.h>


@interface ThreadGlue : NSObject
{
    void* (*mThreadFunc)(void*);
    void* mArg;
}

- (id)initWithFunc:(void* (*)(void*))func andArg:(void*)arg;
- (void)threadFunc:(id)object;

@end

@implementation ThreadGlue

- (id)initWithFunc:(void* (*)(void*))func andArg:(void*)arg
{
	if(!(self = [super init]))
		return self;
        
    mThreadFunc = func;
    mArg = arg;
    
    return self;
}

- (void)threadFunc:(id)object
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    [NSThread setThreadPriority:0.0];
    
    mThreadFunc(mArg);
    
    [pool release];
}

@end

namespace pammo
{

void spawnThread(void* (*func)(void*), void* arg)
{
    ThreadGlue* glue = [[ThreadGlue alloc] initWithFunc:func andArg:arg];
    
    NSThread *thread = [[NSThread alloc] initWithTarget:glue selector:@selector(threadFunc:) object:nil];
    [thread start];
}

void dprintf(char const* format, ...)
{
	va_list args;
	va_start(args, format);
	
	vprintf(format, args);
    printf("\n");
    
	va_end(args);
}

Vector2 getFrameSize()
{
    return Vector2(480, 320);
}

void openRawImagePVRTC(char const* path, RawImage* image)
{
    FILE* f = fopen(path, "rb");
    
    // Read the entire map into memory.
    fseek(f, 0, SEEK_END);
    size_t s = ftell(f);
    rewind(f);
    
    uint8_t* buffer = new uint8_t[s];
    fread(buffer, s, 1, f);
    fclose(f);
    
    image->mSize.x = sqrt(s*2);
    image->mSize.y = image->mSize.x;
    image->mPixelSize = image->mSize;
    image->mBytesPerPixel = 0;
    image->mPixels = buffer;
}

void openRawImagePNG(char const* path, RawImage* image)
{
    CGImageRef spriteImage;
	CGContextRef spriteContext;
    
	NSString* s = [[NSString alloc] initWithCString:path];
	UIImage* uiImage = [UIImage imageNamed:s];
	spriteImage = uiImage.CGImage;
    
    // Load image x, y and byte size.
    uint32_t x = CGImageGetWidth(spriteImage);
    uint32_t y = CGImageGetHeight(spriteImage);
    image->mBytesPerPixel = CGImageGetBitsPerPixel(spriteImage)/8;
    if(!x || !y)
    {
        assert(0);
    }
    // Set image size.
	image->mSize.x = x;
	image->mSize.y = y;
    
    // Check if x is power of two. If not, bump it up.
    if((x & (x-1)) != 0)
    {
        x--;
        x = (x >> 1) | x;
        x = (x >> 2) | x;
        x = (x >> 4) | x;
        x = (x >> 8) | x;
        x = (x >> 16) | x;
        x++;
    }
    
    // Check if y is power of two. If not, bump it up.
    if((y & (y-1)) != 0)
    {
        y--;
        y = (y >> 1) | y;
        y = (y >> 2) | y;
        y = (y >> 4) | y;
        y = (y >> 8) | y;
        y = (y >> 16) | y;
        y++;
    }
    
    // Set pixel size.
    image->mPixelSize.x = x;
    image->mPixelSize.y = y;
	
	// Allocated memory needed for the bitmap context
    uint32_t pixelBytes = image->mPixelSize.x * image->mPixelSize.y * 4;
	image->mPixels = new uint8_t[pixelBytes];
    memset(image->mPixels, 0, pixelBytes);
    
    // Draw image into the buffer.
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	spriteContext = CGBitmapContextCreate(image->mPixels, image->mPixelSize.x, image->mPixelSize.y, 8,
                                          image->mPixelSize.x * 4, colorSpace, kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, image->mSize.x, image->mSize.y), spriteImage);
    
    // If this is 32bit, undoe the horrors of premultiplied alpha.
    if(image->mBytesPerPixel == 4)
    {
        for(uint32_t i=0; i < image->mPixelSize.x * image->mPixelSize.y; ++i)
        {
            uint32_t alpha = image->mPixels[i*4 + 3];
            if(alpha == 0) continue;
            image->mPixels[i*4 + 0] = image->mPixels[i*4 + 0] * 255 / alpha;
            image->mPixels[i*4 + 1] = image->mPixels[i*4 + 1] * 255 / alpha;
            image->mPixels[i*4 + 2] = image->mPixels[i*4 + 2] * 255 / alpha;
        }
    }
    
    // If this image is only 3 component, we need to downsample.
    if(image->mBytesPerPixel == 3)
    {
        // Convert down to 16bit.
        /*image->mBytesPerPixel = 2;
        for(uint32_t yi=0; yi<image->mSize.y; ++yi)
        {
            // Going down to 16bit.
            uint8_t* src = image->mPixels + (uint32_t)(yi*image->mPixelSize.x*4);
            uint16_t* dst = (uint16_t*)(image->mPixels + (uint32_t)(yi*image->mPixelSize.x*2));
            for(uint32_t xi=0; xi<image->mSize.x; ++xi)
            {
                uint32_t r = src[xi*4 + 0] * 31 / 255;
                uint32_t g = src[xi*4 + 1] * 63 / 255;
                uint32_t b = src[xi*4 + 2] * 31/ 255;
                
                dst[xi] = r << 11 | g << 5 | b;
            }
        }*/
        
        // Convert down to 24bit.
        for(uint32_t yi=0; yi<image->mPixelSize.y; ++yi)
        {
            // Going down to 24bit.
            uint8_t* src = image->mPixels + (uint32_t)(yi*image->mPixelSize.x*4);
            uint8_t* dst = image->mPixels + (uint32_t)(yi*image->mPixelSize.x*3);
            for(uint32_t xi=0; xi<image->mPixelSize.x; ++xi)
            {
                dst[xi*3 + 0] = src[xi*4 + 0];
                dst[xi*3 + 1] = src[xi*4 + 1];
                dst[xi*3 + 2] = src[xi*4 + 2];
            }
        }
        
    }
    
	CGContextRelease(spriteContext);
    CGColorSpaceRelease(colorSpace);
}

void openRawImage(char const* path, RawImage* image)
{
    if(strstr(path, ".pvrtc") != 0)
    {
        openRawImagePVRTC(path, image);
    }
    else if(strstr(path, ".png") != 0)
    {
        openRawImagePNG(path, image);
    }
    else
    {
        assert(0);
    }
}

uint64_t getTime(void)
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000 + t.tv_usec;
}
	
    void alEnumToString(char const* str, int e);

    
#define AUDIO_BUFFER_SIZE 10240 //65536
    
int openAudio_platform(AudioInstance* instance)
{
    char basePath[1024] = {0};
    [[[NSBundle mainBundle] bundlePath] getCString:basePath maxLength:1024 encoding:NSASCIIStringEncoding];
    char fullPath[1024];
    sprintf(fullPath, "%s/data/%s", basePath, instance->mClip->mPath);
//    dprintf("%s", fullPath);
    CFURLRef url = NULL;
    url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8 const*)fullPath, strlen(fullPath), false);
    if(!url)
        return -1;

    OSStatus result = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &instance->mAudio.mFile);
    if(result < 0)
    {
        return -1;
    }

    UInt32 size = sizeof(instance->mAudio.mNumPackets);
    result = AudioFileGetProperty(instance->mAudio.mFile, kAudioFilePropertyAudioDataPacketCount, &size, &instance->mAudio.mNumPackets);
    if(result < 0)
    {
        return -1;
    }

    size = sizeof(instance->mAudio.mMaxPacketSize);
    result = AudioFileGetProperty(instance->mAudio.mFile, kAudioFilePropertyMaximumPacketSize, &size, &instance->mAudio.mMaxPacketSize);
    if(result < 0)
    {
        return -1;
    }

    size = sizeof(instance->mAudio.mFormat);
    result = AudioFileGetProperty(instance->mAudio.mFile, kAudioFilePropertyDataFormat, &size, &instance->mAudio.mFormat);
    if(result < 0)
    {
        return -1;
    }
    
    switch(instance->mAudio.mFormat.mBitsPerChannel)
    {
        case 8:
            if(instance->mAudio.mFormat.mChannelsPerFrame == 1)
                instance->mFormat = AL_FORMAT_MONO8;
            else
                instance->mFormat = AL_FORMAT_STEREO8;
            break;
        case 16:
            if(instance->mAudio.mFormat.mChannelsPerFrame == 1)
                instance->mFormat = AL_FORMAT_MONO16;
            else
                instance->mFormat = AL_FORMAT_STEREO16;
            break;
        default:
            dprintf("WTF");
            return -1;
    }            

    instance->mSampleRate = instance->mAudio.mFormat.mSampleRate;
    
    
    instance->mAudio.mCurrentPacket = 0;
    instance->mAudio.mCurrentByte = 0;

    int bytes;
    if(instance->mFormat == AL_FORMAT_STEREO16 || instance->mFormat == AL_FORMAT_MONO16)
        bytes = 2;
    else if(instance->mFormat == AL_FORMAT_STEREO8 || instance->mFormat == AL_FORMAT_MONO8)
        bytes = 1;

    instance->mAudio.mReadFreq = ( (float)AUDIO_BUFFER_SIZE/ (float)(instance->mSampleRate * bytes) ) * 1000000.0f;
    instance->mAudio.mReadFreq -= 40000; // estimated HW latency to prevent underflows.
    instance->mAudio.mNextRead = getTime();
    
    return 0;
}

void stopAudio_platform(AudioInstance* instance)
{
    instance->mAudio.mCurrentPacket = 0;
    instance->mAudio.mCurrentByte  = 0;
}
    
void closeAudio_platform(AudioInstance* instance)
{
    // return any buffers.
    for(AudioBuffer* curr = instance->mBuffersHead; curr; curr = curr->mNext)
    {
        gAudioLibrary->returnAudioBuffer(curr);
    }

    AudioFileClose(instance->mAudio.mFile);    
}

void updateAudio_platform(AudioInstance* instance)
{
    uint64_t now = getTime();
    if(now < instance->mAudio.mNextRead)
        return;
    
    UInt32 numBytes = AUDIO_BUFFER_SIZE;

    uint32_t elapsed = now - instance->mAudio.mNextRead;
    if(elapsed > instance->mAudio.mReadFreq)
    {
        dprintf("approaching underrun");
        numBytes += ((AUDIO_BUFFER_SIZE/instance->mAudio.mReadFreq) * (elapsed - instance->mAudio.mNextRead));
    }
    
    
    instance->mAudio.mNextRead = now + instance->mAudio.mReadFreq;
    
    
    

    uint8_t* buf =  (uint8_t*)alloca(numBytes);
    
	OSStatus ret;
	ret = AudioFileReadBytes(instance->mAudio.mFile, false, instance->mAudio.mCurrentByte, &numBytes, buf);
	if(ret < 0 ) // error
	{
        if(ret == -39) // EOF
        {
            instance->mPlaysRemain--;
            if(instance->mPlaysRemain>0)
            {
                instance->mAudio.mCurrentByte = 0;
                instance->mState = AudioInstance::ReadyToPlay;
            }
            else
            {
                // No more reading.
                instance->mAudio.mNextRead = 0xffffffffffffffffull;
            }
        }    
        else
        {
            dprintf("fillCurrentAudioBuffer failed to read %d", ret);
            return;
        }
	}
    
    if(numBytes)
    {
        instance->mAudio.mCurrentByte+= numBytes;
        AudioBuffer* b = gAudioLibrary->getAudioBuffer();
        if(!b)
        {
            dprintf("Out of audio buffers!");
            return;
        }
        
        alBufferData(b->mBuffer, instance->mFormat, buf, numBytes, instance->mSampleRate);
//        dprintf("Q %u on %u", b->mBuffer, instance->mSource);
        alSourceQueueBuffers(instance->mSource, 1, &b->mBuffer);
        if(instance->mState == AudioInstance::ReadyToPlay)
        {
            alSourcePlay(instance->mSource);        
            instance->mState = AudioInstance::Playing;
        }
        
        if(!instance->mBuffersHead)
        {
//            dprintf("head insert");
            instance->mBuffersHead = b;
            instance->mBuffersTail = b;
        }
        else
        {
            instance->mBuffersTail->mNext = b;
            instance->mBuffersTail = b;
        }
        
        b->mNext = NULL;
    }
    else
    {
        dprintf("numbytes is zero!");
    }
    
    
    ALint state;
    alGetSourcei(instance->mSource, AL_SOURCE_STATE, &state);
    
    // Stopped but still buffers.
    if(state == AL_STOPPED && instance->mBuffersHead)
    {
        dprintf("Buffer underrun. Restarting stream.");
        alSourcePlay(instance->mSource);
    }
}    

    
}
