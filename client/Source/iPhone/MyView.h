//
//  MyView.h
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "aspenGraphicsContext.h"

#import "entity.h"
#import "world.h"
#include "camera.h"

@interface MyView : UIView
{
	AspenGraphicsContext* mContext;
	World* mWorld;
	NSTimer* mTimer;
	Entity* mEntity;
	
	UIButton* mNormalButton;
	UIButton* mScaleRotButton;
	UIButton* mTranslateButton;
	UIImageView* mTouchIndicator;
	bool mDraggingTouchIndicator;

	enum InputMode
	{
		InputModeNormal,
		InputModeScaleRot,
		InputModeTranslate
	};
	
	InputMode mInputMode;
	CGPoint mLastTouch;
	
	bool mMoving;
	CGPoint mMovingTo;
}

- (void)buildWorld:(CGRect) frame;
- (void)buildInputUI:(CGRect) frame;

- (void)clearInputMode;
- (void)setNormalInputMode:(id) button;
- (void)setScaleRotInputMode:(id) button;
- (void)setTranslateInputMode:(id) button;

@end
