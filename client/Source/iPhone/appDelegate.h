#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class MainView;

@interface AppDelegate : NSObject
{
    UIWindow *mWindow;
    MainView *mMainView;
}

@property (nonatomic, retain) UIWindow *mWindow;
@property (nonatomic, retain) MainView *mMainView;

@end
