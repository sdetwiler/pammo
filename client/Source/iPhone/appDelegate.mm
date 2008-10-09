#import "appDelegate.h"
#import "MainView.h"

int main(int argc, char *argv[])
{
	// Set cwd.
	size_t len = strlen(argv[0]);
	while(len != 0 && argv[0][len] != '/')
		--len;
	char* homeDir = new char[len+1];
	strncpy(homeDir, argv[0], len);
	homeDir[len+1] = 0;
	chdir(homeDir);
	delete[] homeDir;
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppDelegate");
    [pool release];
    return retVal;
}

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

- (void)applicationWillTerminate:(UIApplication *)application
{
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    gGame->lowMemory();
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

