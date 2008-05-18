//
//  MyView.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "MyView.h"

#import "world.h"
#import "camera.h"
#import "entity.h"
#import "builder.h"

@implementation MyView

- (id)initWithFrame:(CGRect)frame
{
	if(!(self = [super initWithFrame:frame]))
		return self;
	
	mTimer = [NSTimer scheduledTimerWithTimeInterval:(1.0 / 20) target:self selector:@selector(onUpdate) userInfo:nil repeats:YES];
	
	[self buildWorld: frame];
	
	[self buildInputUI: frame];
	[self setNormalInputMode:0];
	
	mMoving = false;
	mDraggingTouchIndicator = false;
	
	return self;
}

- (void)dealloc
{
	//CGLayerRelease(mLayer);
	//CGContextRelease(mContext);
	
	[super dealloc];
}

- (void)buildWorld:(CGRect) frame
{
	mContext = new AspenGraphicsContext();
	mWorld = new World(mContext);
	
	mEntity = 0;
	
	builder(mWorld);
}

- (void)drawRect:(CGRect)clip
{
	mWorld->draw();
}

- (void)onUpdate
{
	//mEntity->mRotation += 0.01;
	//mEntity->makeDirty();
	
	//mWorld->mCamera->mRotation += 0.01;
	//mWorld->mCamera->makeDirty();
	
	//static float xScale = 320/2;
	//static float yScale = 480/2;
	//mWorld->mCamera->mScale.width = xScale;
	//mWorld->mCamera->mScale.height = yScale;
	//mWorld->mCamera->makeDirty();
	//xScale += 6;
	//if(xScale > 320*2)
	//	xScale = 320/2;
	//yScale += 6;
	//if(yScale > 480*2)
	//	yScale = 480/2;
	
	if(mMoving)
	{
		float dir = atan2(mMovingTo.y - mEntity->mCenter.y, mMovingTo.x - mEntity->mCenter.x);
		
		float distance = sqrt(pow(mEntity->mCenter.x - mMovingTo.x, 2) + pow(mEntity->mCenter.y - mMovingTo.y, 2));
		if(distance > 3)
		{
			distance = 3;
		}
		else
		{
			mMoving = false;
		}
		
		mEntity->mCenter.x += cos(dir) * distance;
		mEntity->mCenter.y += sin(dir) * distance;
		mEntity->mRotation = dir;
		mEntity->makeDirty();
			
		mWorld->mCamera->mCenter.x += cos(dir)*distance;
		mWorld->mCamera->mCenter.y += sin(dir)*distance;
		mWorld->mCamera->makeDirty();
	}
	
	[self setNeedsDisplay];
}

static bool pointInside(UIView* view, CGPoint point)
{
	if(point.x >= view.frame.origin.x
	  && point.x < view.frame.origin.x + view.frame.size.width
	  && point.y >= view.frame.origin.y
	  && point.y < view.frame.origin.y + view.frame.size.height)
		return YES;
	return NO;
}

- (void)touchesChangedWithEvent:(UIEvent *)event
{
	dprintf("Touch Event");

    NSSet *touches = [event allTouches];
    for(UITouch *touch in touches)
    {
        CGPoint touchLocation = [touch locationInView:self];
		
		dprintf("  - %f, %f - %d", touchLocation.x, touchLocation.y, touch.phase);
		
		if(!mTouchIndicator.hidden && touch.phase == UITouchPhaseBegan && pointInside(mTouchIndicator, touchLocation))
		{
			mDraggingTouchIndicator = true;
			mTouchIndicator.center = touchLocation;
			continue;
		}
		
		if(mDraggingTouchIndicator && touch.phase == UITouchPhaseMoved)
		{
			mTouchIndicator.center = touchLocation;
			continue;
		}
		
		if(mDraggingTouchIndicator && touch.phase == UITouchPhaseEnded)
		{
			mDraggingTouchIndicator = false;
			continue;
		}
		
		switch(mInputMode)
		{
		case InputModeNormal:
			if(touch.phase == UITouchPhaseBegan || touch.phase == UITouchPhaseMoved)
			{
				//*** mMovingTo = mWorld->mCamera->translateToWorld(touchLocation);
				mMoving = true;
			}
			break;
		case InputModeScaleRot:
			if(touch.phase == UITouchPhaseBegan)
			{
				mLastTouch = touchLocation;
			}
			if(touch.phase == UITouchPhaseMoved)
			{
				//CGPoint newLocation = mWorld->mCamera->translateToWorld(touchLocation);
				//CGPoint lastLocation = mWorld->mCamera->translateToWorld(mLastTouch);
				//mWorld->mCamera->mSize.width += (newLocation.x - lastLocation.x)*2;
				//mWorld->mCamera->mSize.height += (newLocation.y - lastLocation.y)*2;
				
				//float newMag = sqrt(pow(newLocation.x - mWorld->mCamera->mSize.width/2, 2) + pow(newLocation.y - mWorld->mCamera->mSize.height/2, 2));
				//float lastMag = sqrt(pow(lastLocation.x - mWorld->mCamera->mSize.width/2, 2) + pow(lastLocation.y - mWorld->mCamera->mSize.height/2, 2));
				
				float newMag = sqrt(pow(touchLocation.x - mTouchIndicator.center.x, 2) + pow(touchLocation.y - mTouchIndicator.center.y, 2));
				float lastMag = sqrt(pow(mLastTouch.x - mTouchIndicator.center.x, 2) + pow(mLastTouch.y - mTouchIndicator.center.y, 2));
				
				float percent = (lastMag - newMag)/480;
				
				//dprintf("New: %.1f Last: %.1f percent: %.3f", newMag, lastMag, percent);
				
				mWorld->mCamera->mSize.x *= percent*4 + 1;
				mWorld->mCamera->mSize.y *= percent*4 + 1;
				
				// Rotation.
				float newDir = atan2(touchLocation.y - mTouchIndicator.center.y, touchLocation.x - mTouchIndicator.center.x);
				float oldDir = atan2(mLastTouch.y - mTouchIndicator.center.y, mLastTouch.x - mTouchIndicator.center.x);
				mWorld->mCamera->mRotation += newDir - oldDir;
				
				mWorld->mCamera->makeDirty();
				mLastTouch = touchLocation;
			}
			break;
		case InputModeTranslate:
			if(touch.phase == UITouchPhaseBegan)
			{
				mLastTouch = touchLocation;
			}
			if(touch.phase == UITouchPhaseMoved)
			{
				//*** CGPoint newLocation = mWorld->mCamera->translateToWorld(touchLocation);
				//*** CGPoint lastLocation = mWorld->mCamera->translateToWorld(mLastTouch);
				CGPoint lastLocation;
				CGPoint newLocation;
				mWorld->mCamera->mCenter.x += lastLocation.x - newLocation.x;
				mWorld->mCamera->mCenter.y += lastLocation.y - newLocation.y;
				mWorld->mCamera->makeDirty();
				mLastTouch = touchLocation;
			}
			break;
		}
    }
}

- (void)buildInputUI:(CGRect) frame
{
	mInputMode = InputModeNormal;
	
	const int toggleHeight = 32;
	const int toggleWidth = 100;
	const int toggleOffset = 16;
	
	// Add toggle button.
	mNormalButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	mNormalButton.frame =  CGRectMake(toggleOffset, frame.size.height - toggleHeight*3 - toggleOffset*3, toggleWidth, toggleHeight);
	[mNormalButton setTitle:@"Normal" forStates: UIControlStateNormal];
    [mNormalButton addTarget:self action:@selector(setNormalInputMode:) forControlEvents:UIControlEventTouchUpInside]; 
	[self addSubview:mNormalButton];
	
	// Add toggle button.
	mScaleRotButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	mScaleRotButton.frame =  CGRectMake(toggleOffset, frame.size.height - toggleHeight*2 - toggleOffset*2, toggleWidth, toggleHeight);
	[mScaleRotButton setTitle:@"ScaleRot" forStates: UIControlStateNormal];
    [mScaleRotButton addTarget:self action:@selector(setScaleRotInputMode:) forControlEvents:UIControlEventTouchUpInside]; 
	[self addSubview:mScaleRotButton];
	
	// Add toggle button.
	mTranslateButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	mTranslateButton.frame =  CGRectMake(toggleOffset, frame.size.height-toggleHeight - toggleOffset, toggleWidth, toggleHeight);
	[mTranslateButton setTitle:@"Translate" forStates: UIControlStateNormal];
    [mTranslateButton addTarget:self action:@selector(setTranslateInputMode:) forControlEvents:UIControlEventTouchUpInside]; 
	[self addSubview:mTranslateButton];
	
	// Add touch indicator.
	UIImage *image = [UIImage imageNamed:@"CyanButton.png"];
	mTouchIndicator = [[UIImageView alloc] initWithImage:image];
	mTouchIndicator.frame =  CGRectMake(155, 235, 10, 10);
	mTouchIndicator.hidden = YES;
	mTouchIndicator.alpha = 0.7;
	mTouchIndicator.userInteractionEnabled = NO;
	[self addSubview:mTouchIndicator];
}

- (void)clearInputMode
{
	[mNormalButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
	[mScaleRotButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
	[mTranslateButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
	mTouchIndicator.hidden = YES;
	mTouchIndicator.center = CGPointMake(160, 240);
}

- (void)setNormalInputMode:(id) button
{
	[self clearInputMode];
	
	[mNormalButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
	
	mInputMode = InputModeNormal;
}

- (void)setScaleRotInputMode:(id) button
{
	[self clearInputMode];
	
	[mScaleRotButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
	mTouchIndicator.hidden = NO;
	
	mInputMode = InputModeScaleRot;
}

- (void)setTranslateInputMode:(id) button
{
	[self clearInputMode];
	
	[mTranslateButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
	
	mInputMode = InputModeTranslate;
}

@end
