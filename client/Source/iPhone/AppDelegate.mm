//
//  FlainAppDelegate.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "AppDelegate.h"
#import "MainView.h"
#import "debugConsole.h"

@implementation AppDelegate

@synthesize mWindow;
@synthesize mMainView;
@synthesize mDebugConsole;

- (void)applicationDidFinishLaunching:(UIApplication *)application
{	
	// Create window
	self.mWindow = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	
	// Create debug console
	self.mDebugConsole = [[[DebugConsole alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
	
	// Create main view
	self.mMainView = [[[MainView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
    
    // Set up main view
	[mWindow addSubview:mMainView];
	
    // Set up debug console
	[mWindow addSubview:mDebugConsole];
    
	// Show window
	[mWindow makeKeyAndVisible];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	[self.mMainView stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	[self.mMainView startAnimation];
}

- (void)dealloc
{
	[mDebugConsole release];
	[mMainView release];
	[mWindow release];
	[super dealloc];
}

@end
