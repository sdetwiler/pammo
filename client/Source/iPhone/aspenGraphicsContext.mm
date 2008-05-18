/*
 *  aspenGraphicsContext.cpp
 *  Pammo
 *
 *  Created by James Marr on 4/20/08.
 *  Copyright 2008 scea. All rights reserved.
 *
 */

#include "aspenGraphicsContext.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

namespace pammo
{

Vector2 AspenImage::getSize()
{
	return mSize;
}

AspenGraphicsContext::AspenGraphicsContext()
{
	mContext = UIGraphicsGetCurrentContext();
}

AspenGraphicsContext::~AspenGraphicsContext()
{
}
		
Vector2 AspenGraphicsContext::getFrameSize()
{
	return Vector2(320, 480);
}
		
void AspenGraphicsContext::frameBegin()
{
	mContext = UIGraphicsGetCurrentContext();
}

void AspenGraphicsContext::frameEnd()
{
}

void AspenGraphicsContext::setCameraTransform(Transform2 const& transform)
{
	//dprintf("Camera Transform\n");
	//dprintf("[ %f %f ]\n", transform[0], transform[1]);
	//dprintf("[ %f %f ]\n", transform[2], transform[3]);
	//dprintf("[ %f %f ]\n", transform[4], transform[5]);

	CGContextConcatCTM(mContext, *(CGAffineTransform*)&transform);
}
		
Image* AspenGraphicsContext::openImage(char const* path)
{
	AspenImage* image = new AspenImage();
	image->mImage = [UIImage imageNamed:@"ground.png"].CGImage;
	image->mSize.x = CGImageGetWidth(image->mImage);
	image->mSize.x = CGImageGetHeight(image->mImage);
	return image;
}

void AspenGraphicsContext::closeIage(Image* image)
{
}

void AspenGraphicsContext::drawImage(Image* image, Transform2 const& transform, Transform2 const& invTransform)
{
	CGContextConcatCTM(mContext, *(CGAffineTransform*)&transform);
	
	CGRect rect;
	rect.origin.x = 0;
	rect.origin.y = 0;
	rect.size.width = 1;
	rect.size.height = 1;
	//rect.size.width = 200;
	//rect.size.height = 200;
	
	CGContextDrawImage(mContext, rect, ((AspenImage*)image)->mImage);
	
	CGContextConcatCTM(mContext, *(CGAffineTransform*)&invTransform);
}		
		
}