//
//  FlainAppDelegate.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "FlainAppDelegate.h"
#import "MyView.h"
#import "debugConsole.h"

@implementation FlainAppDelegate

@synthesize mWindow;
@synthesize mContentView;
@synthesize mDebugConsole;

- (void)applicationDidFinishLaunching:(UIApplication *)application
{	
	// Create window
	self.mWindow = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
	
	// Create debug console
	self.mDebugConsole = [[[DebugConsole alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
	
	// Create content view
	self.mContentView = [[[MyView alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]] autorelease];
    
    // Set up content view
	[mWindow addSubview:mContentView];
	
    // Set up debug console
	[mWindow addSubview:mDebugConsole];
    
	// Show window
	[mWindow makeKeyAndVisible];
}

- (void)dealloc
{
	[mDebugConsole release];
	[mContentView release];
	[mWindow release];
	[super dealloc];
}

@end
