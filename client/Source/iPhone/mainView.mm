#import "mainView.h"

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
    
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
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
	mTimer = [NSTimer scheduledTimerWithTimeInterval:1./30. target:self selector:@selector(drawView) userInfo:nil repeats:YES];
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
    uint64_t delta = t - mLastTime;
    mLastTime = t;
    
    // If callback is not within a fudge area, do nothing.
    uint64_t fudge = 3000;
    if(delta < 33333-fudge || delta > 33333+fudge)
    {
        return;
    }
    
    #if 1
    static uint32_t slots[50];
    static uint32_t cur = 0;
    
    slots[cur] = delta;
    ++cur;
    if(cur == 50)
    {
        uint64_t total = 0;
        for(uint32_t i=0; i < 50; ++i)
        {
            //printf("%d, ", slots[i]);
            total += slots[i];
        }
        printf("average: %d\n", total / 50);
        cur = 0;
    }
    #endif
	
	[EAGLContext setCurrentContext:mContext];
	[mContext presentRenderbuffer:GL_RENDERBUFFER_OES];
	
    //uint64_t updateStart = getTime();
	mGame->update();
	mGame->draw();
    //uint64_t updateEnd = getTime();
    
    #if 0
    static uint32_t slots[50];
    static uint32_t cur = 0;
    
    slots[cur] = updateEnd - updateStart;
    ++cur;
    if(cur == 50)
    {
        uint64_t total = 0;
        for(uint32_t i=0; i < 50; ++i)
        {
            total += slots[i];
        }
        printf("average: %d\n", total / 50);
        cur = 0;
    }
    #endif
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	[self touchesMoved:touches withEvent:event];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
	for(UITouch *touch in touches)
	{
        pammo::Touch output;
        
		CGPoint touchLocation = [touch locationInView:self];
        output.mLocation.x = touchLocation.y;
		output.mLocation.y = 320 - touchLocation.x;
        output.mSerialNumber = touch;
        
		if(touch.phase == 0) output.mPhase = pammo::Touch::PhaseBegin;
		if(touch.phase == 1) output.mPhase = pammo::Touch::PhaseMove;
		if(touch.phase == 3) output.mPhase = pammo::Touch::PhaseEnd;
        
        //dprintf("Touch %s - %f, %f - %p", touch.phase==0?"Begin":touch.phase==1?"Move":"End", output.mLocation.x, output.mLocation.y, output.mSerialNumber);
        
        mGame->touches(1, &output);
	}
}

- (void) touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
	[self touchesMoved:touches withEvent:event];
}

@end
