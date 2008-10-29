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
    
    // If this image is only 3 component, we need to downsample.
    if(image->mBytesPerPixel == 3)
    {
        // Convert down to 16bit.
        image->mBytesPerPixel = 2;
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
        }
        
        // Convert down to 24bit.
        //for(uint32_t yi=0; yi<image->mSize.y; ++yi)
        //{
            // Going down to 24bit.
            //uint8_t* src = image->mPixels + (uint32_t)(yi*image->mPixelSize.x*4);
            //uint8_t* dst = image->mPixels + (uint32_t)(yi*image->mPixelSize.x*3);
            //for(uint32_t xi=0; xi<image->mSize.x; ++xi)
            //{
            //    dst[xi*3 + 0] = src[xi*4 + 0];
            //    dst[xi*3 + 1] = src[xi*4 + 1];
            //    dst[xi*3 + 2] = src[xi*4 + 2];
            //}
        //}
        
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
    dprintf("%s", fullPath);
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
    instance->mAudio.mReadFreq -= 50000; // estimated HW latency to prevent underflows.
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
    
    instance->mAudio.mNextRead = now + instance->mAudio.mReadFreq;
    
    
    
    UInt32 numBytes = AUDIO_BUFFER_SIZE;
//    UInt32 numPackets = 1024;//instance->mAudio.mNumPackets;

//    AudioStreamPacketDescription* desc = (AudioStreamPacketDescription*)alloca(sizeof(AudioStreamPacketDescription)*numPackets);
//    uint8_t* buf = (uint8_t*)alloca(numPackets * instance->mAudio.mMaxPacketSize);

    uint8_t* buf =  (uint8_t*)alloca(numBytes);
    
	OSStatus ret;
//    dprintf("reading");
	ret = AudioFileReadBytes(instance->mAudio.mFile, false, instance->mAudio.mCurrentByte, &numBytes, buf);
//    dprintf("%d", ret);
	if(ret < 0 && ret !=-39) // error that isn't EOF
	{
        dprintf("fillCurrentAudioBuffer failed to read %d", ret);
		return;
	}
    if(numBytes)
    {
//        dprintf("numBytes %d", numBytes);
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
    
    
    ALint state;
    alGetSourcei(instance->mSource, AL_SOURCE_STATE, &state);
    
    // Stopped but still buffers.
    if(state == AL_STOPPED && instance->mBuffersHead)
    {
        dprintf("Buffer underrun. Restarting stream.");
        alSourcePlay(instance->mSource);
    }
    
    /*********
    AudioFileReadPackets(instance->mAudio.mFile,
                            false,
                            &numBytes,
                            desc,
                            instance->mAudio.mCurrentPacket,
                            &numPackets,
                            buf
                        );

    if(numPackets > 0)
    {
        uint32_t offset = 0;
        // Generate an ALBuffer for each packet in the buffer.
        for(uint32_t i=0; i<numPackets; ++i)
        {
            AudioBuffer* b = gAudioLibrary->getAudioBuffer();
            if(!b)
                return;

// SCD: is mStartOffset from the beginning of buf or within the current buffer?                
            alBufferData(b->mBuffer, AL_FORMAT_STEREO16, buf+desc[i].mStartOffset, desc[i].mDataByteSize, 44100);
    	    alSourceQueueBuffers(instance->mSource, 1, &b->mBuffer);
            
            // Store the buffer with this instance for later return.
            b->mNext = instance->mBuffers;
            instance->mBuffers = b;
        }
                   
        instance->mAudio.mCurrentPacket+=numPackets;
    }
     ******/

}    


/************************************************    
ALCdevice* gALDevice = NULL;
ALCcontext* gALContext = NULL;
int initializeAudio()
{
	gALDevice = alcOpenDevice(NULL);
	if(!gALDevice)
		return -1;

    int error;
	
    gALContext = alcCreateContext(gALDevice, NULL);
    alcMakeContextCurrent(gALContext);
 	error = alGetError();
    if(error)
    {
        alEnumToString("makeContextCurrent", error);
        return -1;
    }
    
    return 0;
    
    alListener3f(AL_POSITION, 0,0,0);
 	error = alGetError();
    if(error)
    {
        alEnumToString("alListener3f position", error);
        return -1;
    }
    
    alListener3f(AL_VELOCITY, 0,0,0);
 	error = alGetError();
    if(error)
    {
        alEnumToString("alListener3f velocity", error);
        return -1;
    }
    
	return 0;
}		
	
void shutdownAudio()
{
    alcDestroyContext(gALContext);
	alcCloseDevice(gALDevice);
}
    
    
	
int loadAudio(char const* filename, Audio* audio)
{
	OSStatus ret=0;
    int error;

	alGenSources(1, &audio->mSource);
	error = alGetError();
    if(error)
    {
        alEnumToString("alGenSources", error);
        return -1;
    }

    
	alGenBuffers(NUM_AUDIO_BUFFERS, &audio->mBuffer[0]);
	error = alGetError();
    if(error)
    {
        alEnumToString("alGenBuffers", error);
        return -1;
    }
    
    
	audio->mCurrentByte = 0;
	audio->mCurrentBuffer = 0;
	audio->mPlaying = false;
	
    char basePath[1024] = {0};
    [[[NSBundle mainBundle] bundlePath] getCString:basePath maxLength:1024 encoding:NSASCIIStringEncoding];
    char fullPath[1024];
    sprintf(fullPath, "%s/%s", basePath, filename);
    dprintf("%s", fullPath);
	CFURLRef url = NULL;
    url = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8 const*)fullPath, strlen(fullPath), false);
    if(!url)
        return -1;
    

	OSStatus result = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &audio->mFile);
    if(result < 0)
    {
        return -1;
    }

	UInt32 size = sizeof(audio->mNumBytes);
	result = AudioFileGetProperty(audio->mFile, kAudioFilePropertyAudioDataByteCount, &size, &audio->mNumBytes);
    if(result < 0)
    {
        return -1;
    }
    

	return 0;
}
    
    
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef ALvoid	AL_APIENTRY	(*alBufferDataStaticProcPtr) (const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq);
ALvoid  alBufferDataStaticProc(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
    static	alBufferDataStaticProcPtr	proc = NULL;
    
    if (proc == NULL) 
    {
        proc = (alBufferDataStaticProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
    
    if (proc)
        proc(bid, format, data, size, freq);
    
    return;
}
    
	
int fillCurrentAudioBuffer(Audio* audio)
{
    //dprintf("fillCurrentAudioBuffer ###");
    
    // Dequeue a buffer.
    ALint numBuffers = 0;
    alGetSourcei(audio->mSource, AL_BUFFERS_PROCESSED, &numBuffers);
    if(numBuffers > 0)
    {
    //    dprintf("D %d", audio->mCurrentBuffer);
        
        alSourceUnqueueBuffers(audio->mSource, 1, &audio->mBuffer[audio->mCurrentBuffer]);
        int error = alGetError();
        if(error)
        {
            alEnumToString("alSourceUnqueueBuffers", error);
            return -1;
        }
    }    
    
    alGetSourcei(audio->mSource, AL_BUFFERS_QUEUED, &numBuffers);
    if(numBuffers == 2)
        return -1;
    
    UInt32 numBytes = 1024 * 64;
    UInt8* buf = (UInt8*)alloca(numBytes);
	OSStatus ret;
	ret = AudioFileReadBytes(audio->mFile, false, audio->mCurrentByte, &numBytes, buf);
	if(ret < 0 && ret !=-39) // error that isn't EOF
	{
        dprintf("fillCurrentAudioBuffer failed to read %d", ret);
		return -1;
	}
	
	if(numBytes == 0) // EOF
	{
        UInt64 zero = 0;
        UInt32 size = sizeof(zero);
        dprintf("fillCurrentAudioBuffer EOF");
		AudioFileSetProperty(audio->mFile, kAudioFilePropertyDataOffset, size, &zero); 
        audio->mCurrentByte = 0;
        
        return -1;
	}
	
	audio->mCurrentByte+= numBytes;
	

    //alBufferData(audio->mBuffer[audio->mCurrentBuffer], AL_FORMAT_STEREO16, buf, numBytes, 44100);
    alBufferDataStaticProc(audio->mBuffer[audio->mCurrentBuffer], AL_FORMAT_STEREO16, buf, numBytes, 44100);
    int error = alGetError();
    if(error)
    {
        alEnumToString("alBufferData", error);
        return -1;
    }
    
 //   dprintf("Q %d", audio->mCurrentBuffer);

	alSourceQueueBuffers(audio->mSource, 1, &audio->mBuffer[audio->mCurrentBuffer]);
    error = alGetError();
    if(error)
    {
        alEnumToString("alSourceQueueBuffers", error);
        return -1;
    }
	audio->mCurrentBuffer = (audio->mCurrentBuffer+1) % NUM_AUDIO_BUFFERS;
//    dprintf("#### CurrentBuffer is now %d", audio->mCurrentBuffer);
	return 0;
		
}
	
void startAudio(Audio* audio, int volume)
{
    if(!audio)
        return;
	audio->mPlaying = true;
	
	// Get the first buffer queued.
	if(fillCurrentAudioBuffer(audio) < 0)
	{
		dprintf("fillCurrentAudioBuffer failed in startAudio.");
		assert(0);
	}
	
//	alSourceQueueBuffers(audio->mSource, 1, &audio->mBuffer[audio->mCurrentBuffer]);
//    int error = alGetError();
//	audio->mCurrentBuffer = (audio->mCurrentBuffer+1) % NUM_AUDIO_BUFFERS;
		
	alSourcePlay(audio->mSource);
    int error = alGetError();
    if(error)
    {
        alEnumToString("alSourcePlay", error);
        return;
    }
}
	
void updateAudio(Audio* audio)
{
	// streamy stream.


	// Refill dequeued buffer.
	if(fillCurrentAudioBuffer(audio) == 0)
	{
	}
	
    
}

void stopAudio(Audio* audio)
{
		
}
void unloadAudio(Audio* audio)
{
	AudioFileClose(audio->mFile);		
}
*****************************************************/	
	
    
}
