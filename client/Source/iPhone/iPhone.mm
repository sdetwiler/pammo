#include "pammo.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <QuartzCore/QuartzCore.h>

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
    
}
