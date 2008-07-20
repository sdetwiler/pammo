#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include "pammo.h"

@interface MainView : UIView
{
	NSTimer* mTimer;
    uint64_t mMicros;
    uint64_t mLastTime;
	
	EAGLContext *mContext;
	GLint mWidth;
	GLint mHeight;
	GLuint mRenderbuffer, mFramebuffer;
	
	pammo::Game* mGame;
}

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;

@end
