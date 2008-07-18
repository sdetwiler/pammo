//
//  MyView.h
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "game.h"

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
