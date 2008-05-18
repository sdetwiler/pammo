//
//  debugConsole.mm
//  Flain
//
//  Created by James Marr on 3/28/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "DebugConsole.h"

#import "dprintf.h"

static DebugConsole* gDebugConsole = 0;

void dprintf(char const* format, ...)
{
	va_list args;
	va_start(args, format);
	
	char composite[2048];
	vsnprintf(composite, sizeof(composite), format, args);
    
	va_end(args);

	[gDebugConsole addOutput:[NSString stringWithCString:composite]];
}

@implementation DebugConsole

- (id)initWithFrame:(CGRect)frame
{
	if(!(self = [super initWithFrame:frame]))
		return self;
	
	gDebugConsole = self;
		
	[self setOpaque:NO];
	//self.userInteractionEnabled = NO;
	//[self setAlpha:0.5];
	
	// Add output text area.
	mOutput = [[UITextView alloc] initWithFrame: CGRectMake(0, frame.size.height/4, frame.size.width, 3*frame.size.height/4)];
	mOutput.backgroundColor = [UIColor colorWithWhite:0 alpha:0.5];
	mOutput.font = [UIFont systemFontOfSize:12];
	mOutput.textColor = [UIColor colorWithWhite:1 alpha:1];
	mOutput.hidden = YES;
	mOutput.editable = NO;
	mOutput.delaysContentTouches = NO;
	mOutput.showsVerticalScrollIndicator = YES;
	mOutput.showsHorizontalScrollIndicator = YES;
	mOutput.bounces = YES;
	[self addSubview:mOutput];

	// Add toggle button.
	const int toggleHeight = 32;
	const int toggleWidth = 100;
	const int toggleOffset = 16;
	mToggleButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	mToggleButton.frame =  CGRectMake(frame.size.width-toggleWidth-toggleOffset, frame.size.height-toggleHeight - toggleOffset, toggleWidth, toggleHeight);
	[mToggleButton setTitle:@"Console" forStates: UIControlStateNormal];
	[mToggleButton setTitleColor:[UIColor colorWithRed:0 green:0 blue:0 alpha:1] forStates: UIControlStateNormal];
    [mToggleButton addTarget:self action:@selector(toggleOutput:) forControlEvents:UIControlEventTouchUpInside]; 
	[self addSubview:mToggleButton];
	
	// Add debug text.
	mDebug =  [[UILabel alloc] initWithFrame:CGRectMake(self.frame.origin.x + 10, self.frame.origin.y, self.frame.size.width - 20, 20)];
	mDebug.font = [UIFont systemFontOfSize:18.];
	mDebug.textColor = [UIColor lightTextColor];
	mDebug.backgroundColor = [UIColor clearColor];
	mDebug.textAlignment = UITextAlignmentCenter;
	mDebug.numberOfLines = 1;
	mDebug.textAlignment =  UITextAlignmentLeft;
	mDebug.text = @"";
	[self addSubview:mDebug];
		
	return self;
}

- (void)dealloc
{
	[super dealloc];
}

- (void)addOutput:(NSString*) text
{
	//int location = mOutput.text.length;
	//mOutput.text = [mOutput.text stringByAppendingFormat:@"\n- %@", text];
	
	NSString* final = [NSString stringWithFormat:@"- %@\n%@", text, mOutput.text];
	
	if(final.length > 2000)
		final = [final substringToIndex:2000];
	
	mOutput.text = final;
	
	//NSRange range;
	//range.location = location;
	//range.length = 0;
	//[mOutput scrollRangeToVisible:range];
}

- (void) toggleOutput: (id)sender
{
	if(mOutput.hidden)
	{
		mOutput.hidden = NO;
		[mToggleButton setTitleColor:[UIColor colorWithRed:1 green:0 blue:0 alpha:1] forStates: UIControlStateNormal];
	}
	else
	{
		mOutput.hidden = YES;
		[mToggleButton setTitleColor:[UIColor colorWithRed:0 green:0 blue:0 alpha:1] forStates: UIControlStateNormal];
	}
}

static bool pointInside(UIView* view, CGPoint point)
{
	if(point.x >= view.frame.origin.x
	  && point.x < view.frame.origin.x + view.frame.size.width
	  && point.y >= view.frame.origin.y
	  && point.y < view.frame.origin.y + view.frame.size.height)
		return YES;
	return NO;
}

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
	//if([mToggleButton pointInside:point withEvent:event])
	if(pointInside(mToggleButton, point))
	{
		//mOutput.text = @"Hit";
		return mToggleButton;
	}
	
	if(!mOutput.hidden && pointInside(mOutput, point))
		return mOutput;
	
	return nil;
}

@end
