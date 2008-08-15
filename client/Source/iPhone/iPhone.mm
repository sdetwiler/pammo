#include "pammo.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <QuartzCore/QuartzCore.h>

namespace pammo
{

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

void openRawImage(char const* path, RawImage* image)
{
	CGImageRef spriteImage;
	CGContextRef spriteContext;
    
	NSString* s = [[NSString alloc] initWithCString:path];
	UIImage* uiImage = [UIImage imageNamed:s];
	spriteImage = uiImage.CGImage;
	image->mSize.x = CGImageGetWidth(spriteImage);
	image->mSize.y = CGImageGetHeight(spriteImage);
    image->mBytesPerPixel = 4;
	
	// Allocated memory needed for the bitmap context
	image->mPixels = new uint8_t[(uint32_t)(image->mSize.x * image->mSize.y * 4)];
    memset(image->mPixels, 0, image->mSize.x*image->mSize.y*4);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	spriteContext = CGBitmapContextCreate(image->mPixels, image->mSize.x, image->mSize.y, 8, image->mSize.x * 4, colorSpace, kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, image->mSize.x, image->mSize.y), spriteImage);
    
	CGContextRelease(spriteContext);
    CGColorSpaceRelease(colorSpace);
}

uint64_t getTime(void)
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000 + t.tv_usec;
}
    
}
