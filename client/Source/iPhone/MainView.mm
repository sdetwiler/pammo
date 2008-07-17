//
//  MyView.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "MainView.h"

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

@interface MainView()

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end

@implementation MainView

// OSX Magic.
+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
	if(!(self = [super initWithFrame:frame]))
		return self;
    
    self.multipleTouchEnabled = YES;
	
	//[self buildInputUI:frame];
	
	CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
								   [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
                                   
	mContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	
	if(!mContext || ![EAGLContext setCurrentContext:mContext] || ![self createFramebuffer])
	{
		[self release];
		return nil;
	}
	
	mGame = new Game();
	mGame->init();
	
	[self startAnimation];
	
	return self;
}

- (BOOL)createFramebuffer
{	
	glGenFramebuffersOES(1, &mFramebuffer);
	glGenRenderbuffersOES(1, &mRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
	[mContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &mWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &mHeight);
	
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	
	return YES;
}

- (void)destroyFramebuffer
{	
	glDeleteFramebuffersOES(1, &mFramebuffer);
	mFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &mRenderbuffer);
	mRenderbuffer = 0;
}

- (void)layoutSubviews
{
	[EAGLContext setCurrentContext:mContext];
	[self destroyFramebuffer];
	[self createFramebuffer];
	[self drawView];
}

- (void)dealloc
{
	[self stopAnimation];
	
	if ([EAGLContext currentContext] == mContext)
	{
		[EAGLContext setCurrentContext:nil];
	}
	
	[mContext release];	
	[super dealloc];
}


- (void)startAnimation
{
	mTimer = [NSTimer scheduledTimerWithTimeInterval:1./15. target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation
{
	mTimer = nil;
}

- (void)drawView
{
	mGame->update();
	[EAGLContext setCurrentContext:mContext];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
	
	mGame->draw();
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
	[mContext presentRenderbuffer:GL_RENDERBUFFER_OES];
	
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	[self touchesMoved:touches withEvent:event];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
	int cur=0;
	pammo::Touch output[2];
	for(UITouch *touch in touches)
	{
		if(cur == 2) break;
		
		CGPoint touchLocation = [touch locationInView:self];
		output[cur].mLocation.x = touchLocation.x;
		output[cur].mLocation.y = touchLocation.y;
		if(touch.phase == 0) output[cur].mPhase = pammo::Touch::PhaseBegin;
		if(touch.phase == 1) output[cur].mPhase = pammo::Touch::PhaseMove;
		if(touch.phase == 3) output[cur].mPhase = pammo::Touch::PhaseEnd;
		++cur;
	}
	
	mGame->touches(cur, output);
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
	[self touchesMoved:touches withEvent:event];
}

//static bool pointInside(UIView* view, CGPoint point)
//{
//	if(point.x >= view.frame.origin.x
//	  && point.x < view.frame.origin.x + view.frame.size.width
//	  && point.y >= view.frame.origin.y
//	  && point.y < view.frame.origin.y + view.frame.size.height)
//		return YES;
//	return NO;
//}

//- (void)touchesChangedWithEvent:(UIEvent *)event
//{
//	dprintf("Touch Event");
//
//    NSSet *touches = [event allTouches];
//    for(UITouch *touch in touches)
//    {
//        CGPoint touchLocation = [touch locationInView:self];
//		
//		dprintf("  - %f, %f - %d", touchLocation.x, touchLocation.y, touch.phase);
//		
//		if(!mTouchIndicator.hidden && touch.phase == UITouchPhaseBegan && pointInside(mTouchIndicator, touchLocation))
//		{
//			mDraggingTouchIndicator = true;
//			mTouchIndicator.center = touchLocation;
//			continue;
//		}
//		
//		if(mDraggingTouchIndicator && touch.phase == UITouchPhaseMoved)
//		{
//			mTouchIndicator.center = touchLocation;
//			continue;
//		}
//		
//		if(mDraggingTouchIndicator && touch.phase == UITouchPhaseEnded)
//		{
//			mDraggingTouchIndicator = false;
//			continue;
//		}
//		
//		switch(mInputMode)
//		{
//		case InputModeNormal:
//			if(touch.phase == UITouchPhaseBegan || touch.phase == UITouchPhaseMoved)
//			{
//				//*** mMovingTo = mWorld->mCamera->translateToWorld(touchLocation);
//				mMoving = true;
//			}
//			break;
//		case InputModeScaleRot:
//			if(touch.phase == UITouchPhaseBegan)
//			{
//				mLastTouch = touchLocation;
//			}
//			if(touch.phase == UITouchPhaseMoved)
//			{
//				//CGPoint newLocation = mWorld->mCamera->translateToWorld(touchLocation);
//				//CGPoint lastLocation = mWorld->mCamera->translateToWorld(mLastTouch);
//				//mWorld->mCamera->mSize.width += (newLocation.x - lastLocation.x)*2;
//				//mWorld->mCamera->mSize.height += (newLocation.y - lastLocation.y)*2;
//				
//				//float newMag = sqrt(pow(newLocation.x - mWorld->mCamera->mSize.width/2, 2) + pow(newLocation.y - mWorld->mCamera->mSize.height/2, 2));
//				//float lastMag = sqrt(pow(lastLocation.x - mWorld->mCamera->mSize.width/2, 2) + pow(lastLocation.y - mWorld->mCamera->mSize.height/2, 2));
//				
//				float newMag = sqrt(pow(touchLocation.x - mTouchIndicator.center.x, 2) + pow(touchLocation.y - mTouchIndicator.center.y, 2));
//				float lastMag = sqrt(pow(mLastTouch.x - mTouchIndicator.center.x, 2) + pow(mLastTouch.y - mTouchIndicator.center.y, 2));
//				
//				float percent = (lastMag - newMag)/480;
//				
//				//dprintf("New: %.1f Last: %.1f percent: %.3f", newMag, lastMag, percent);
//				
//				mWorld->mCamera->mSize.x *= percent*4 + 1;
//				mWorld->mCamera->mSize.y *= percent*4 + 1;
//				
//				// Rotation.
//				float newDir = atan2(touchLocation.y - mTouchIndicator.center.y, touchLocation.x - mTouchIndicator.center.x);
//				float oldDir = atan2(mLastTouch.y - mTouchIndicator.center.y, mLastTouch.x - mTouchIndicator.center.x);
//				mWorld->mCamera->mRotation += newDir - oldDir;
//				
//				mWorld->mCamera->makeDirty();
//				mLastTouch = touchLocation;
//			}
//			break;
//		case InputModeTranslate:
//			if(touch.phase == UITouchPhaseBegan)
//			{
//				mLastTouch = touchLocation;
//			}
//			if(touch.phase == UITouchPhaseMoved)
//			{
//				//*** CGPoint newLocation = mWorld->mCamera->translateToWorld(touchLocation);
//				//*** CGPoint lastLocation = mWorld->mCamera->translateToWorld(mLastTouch);
//				CGPoint lastLocation;
//				CGPoint newLocation;
//				mWorld->mCamera->mCenter.x += lastLocation.x - newLocation.x;
//				mWorld->mCamera->mCenter.y += lastLocation.y - newLocation.y;
//				mWorld->mCamera->makeDirty();
//				mLastTouch = touchLocation;
//			}
//			break;
//		}
//    }
//}
//
//- (void)buildInputUI:(CGRect) frame
//{
//	const int toggleHeight = 32;
//	const int toggleWidth = 100;
//	const int toggleOffset = 16;
//	
//	// Add toggle button.
//	mTapButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
//	mTapButton.frame =  CGRectMake(toggleOffset, frame.size.height - toggleHeight - toggleOffset, toggleWidth, toggleHeight);
//	[mTapButton setTitle:@"Tap" forState: UIControlStateNormal];
//    [mTapButton addTarget:self action:@selector(tapButton:) forControlEvents:UIControlEventTouchUpInside]; 
//	[self addSubview:mTapButton];
//}
//
//- (void)tapButton:(id) button
//{
//	pammo::Touch touches[2];
//	touches[0].mLocation.x = 0;
//	touches[0].mLocation.y = 0;
//	touches[1].mLocation.x = 1;
//	touches[1].mLocation.y = 1;
//	
//	touches[0].mPhase = touches[1].mPhase = pammo::Touch::PhaseBegin;
//	mGame->touches(2, touches);
//	
//	touches[0].mPhase = touches[1].mPhase = pammo::Touch::PhaseEnd;
//	mGame->touches(2, touches);
//}
//
//- (void)clearInputMode
//{
//	[mNormalButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
//	[mScaleRotButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
//	[mTranslateButton setTitleColor:[UIColor colorWithWhite:0 alpha:1] forStates:UIControlStateNormal];
//	mTouchIndicator.hidden = YES;
//	mTouchIndicator.center = CGPointMake(160, 240);
//}
//
//- (void)setNormalInputMode:(id) button
//{
//	[self clearInputMode];
//	
//	[mNormalButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
//	
//	mInputMode = InputModeNormal;
//}
//
//- (void)setScaleRotInputMode:(id) button
//{
//	[self clearInputMode];
//	
//	[mScaleRotButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
//	mTouchIndicator.hidden = NO;
//	
//	mInputMode = InputModeScaleRot;
//}
//
//- (void)setTranslateInputMode:(id) button
//{
//	[self clearInputMode];
//	
//	[mTranslateButton setTitleColor:[UIColor colorWithRed: 1 green:0 blue:0 alpha:1] forStates:UIControlStateNormal];
//	
//	mInputMode = InputModeTranslate;
//}

@end
