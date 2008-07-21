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

Image* openImage(char const* path)
{
	Image* image = new Image();
	
	CGImageRef spriteImage;
	CGContextRef spriteContext;
	GLubyte *spriteData;
	NSString* s = [[NSString alloc] initWithCString:path];
	UIImage* uiImage = [UIImage imageNamed:s];
	spriteImage = uiImage.CGImage;
	image->mSize.x = CGImageGetWidth(spriteImage);
	image->mSize.y = CGImageGetHeight(spriteImage);
	
	// Allocated memory needed for the bitmap context
	spriteData = (GLubyte *) malloc(image->mSize.x * image->mSize.y * 4);
    memset(spriteData, 0, image->mSize.x*image->mSize.y*4);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	spriteContext = CGBitmapContextCreate(spriteData, image->mSize.x, image->mSize.y, 8, image->mSize.x * 4, colorSpace, kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, image->mSize.x, image->mSize.y), spriteImage);
	
	// Use OpenGL ES to generate a name for the texture.
	glGenTextures(1, &image->mTexture);
	glBindTexture(GL_TEXTURE_2D, image->mTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->mSize.x, image->mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
	
	CGContextRelease(spriteContext);
    CGColorSpaceRelease(colorSpace);
	free(spriteData);
	return image;
}

uint64_t getTime(void)
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000000 + t.tv_usec;
}
    
}
