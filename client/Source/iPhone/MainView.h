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
	
	EAGLContext *mContext;
	GLint mWidth;
	GLint mHeight;
	GLuint mRenderbuffer, mFramebuffer;
	
	pammo::Game* mGame;
	
	UIButton* mTapButton;
	//UIButton* mScaleRotButton;
	//UIButton* mTranslateButton;
	//UIImageView* mTouchIndicator;
	//bool mDraggingTouchIndicator;

	//enum InputMode
	//{
	//	InputModeNormal,
	//	InputModeScaleRot,
	//	InputModeTranslate
	//};
	
	//InputMode mInputMode;
	//CGPoint mLastTouch;
	
	//bool mMoving;
	//CGPoint mMovingTo;
}

//- (void)buildWorld:(CGRect) frame;
//- (void)buildInputUI:(CGRect) frame;
//- (void)tapButton:(id) button;

//- (void)clearInputMode;
//- (void)setNormalInputMode:(id) button;
//- (void)setScaleRotInputMode:(id) button;
//- (void)setTranslateInputMode:(id) button;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView;

@end
