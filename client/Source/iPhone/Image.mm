/*
 *  aspenGraphicsContext.cpp
 *  Pammo
 *
 *  Created by James Marr on 4/20/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "image.h"

#import <OpenGLES/ES1/gl.h>
#import <QuartzCore/QuartzCore.h>

namespace pammo
{

Vector2 getFrameSize()
{
    return Vector2(320, 480);
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
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); // scale linearly when image bigger than texture
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); // scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->mSize.x, image->mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
	
	CGContextRelease(spriteContext);
    CGColorSpaceRelease(colorSpace);
	free(spriteData);
	return image;
}
    
}
