//
//  FlainAppDelegate.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "AppDelegate.h"
#import "MainView.h"

@implementation AppDelegate

@synthesize mWindow;
@synthesize mMainView;

- (void)applicationDidFinishLaunching:(UIApplication *)application
{	
	// Create window
	self.mWindow = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	
	// Create main view
	self.mMainView = [[[MainView alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    // Set up main view
	[mWindow addSubview:mMainView];
    
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
	[mMainView release];
	[mWindow release];
	[super dealloc];
}

@end

