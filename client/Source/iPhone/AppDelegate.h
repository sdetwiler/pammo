//
//  FlainAppDelegate.h
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
	
@class MainView;
@class DebugConsole;

@interface AppDelegate : NSObject
{
    UIWindow *mWindow;
    MainView *mMainView;
	DebugConsole* mDebugConsole;
}

@property (nonatomic, retain) UIWindow *mWindow;
@property (nonatomic, retain) MainView *mMainView;
@property (nonatomic, retain) DebugConsole *mDebugConsole;

@end
