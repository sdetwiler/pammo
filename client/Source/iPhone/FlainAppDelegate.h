//
//  FlainAppDelegate.h
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
	
@class MyView;
@class DebugConsole;

@interface FlainAppDelegate : NSObject
{
    UIWindow *mWindow;
    MyView *mContentView;
	DebugConsole* mDebugConsole;
}

@property (nonatomic, retain) UIWindow *mWindow;
@property (nonatomic, retain) MyView *mContentView;
@property (nonatomic, retain) DebugConsole *mDebugConsole;

@end
