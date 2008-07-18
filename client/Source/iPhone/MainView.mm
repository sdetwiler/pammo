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
	mTimer = [NSTimer scheduledTimerWithTimeInterval:1./60. target:self selector:@selector(drawView) userInfo:nil repeats:YES];
    mLastTime = getTime();
    mMicros = 0;
}


- (void)stopAnimation
{
	mTimer = nil;
}

- (void)drawView
{
    uint64_t t = getTime();
    mMicros += t - mLastTime;
    mLastTime = t;
    
    bool draw = false;
    
    while(mMicros >= 33333)
    {
        mGame->update();
        mMicros -= 33333;
        
        draw = true;
    }
    
    if(draw)
    {
        [EAGLContext setCurrentContext:mContext];
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFramebuffer);
	
        mGame->draw();
	
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderbuffer);
        [mContext presentRenderbuffer:GL_RENDERBUFFER_OES];
    }
	
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

@end
