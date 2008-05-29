//
//  main.m
//  Flain
//
//  Created by James Marr on 3/27/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

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
