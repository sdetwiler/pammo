//
//  debugConsole.h
//  Flain
//
//  Created by James Marr on 3/28/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
	
@interface DebugConsole : UIView
{
	UILabel *mDebug;
	UITextView* mOutput;
	UIButton *mToggleButton;
}

- (void)addOutput:(NSString*) text;

@end
