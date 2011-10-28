//
//  MemBenchNativeIOSAppDelegate.h
//  MemBenchNativeIOS
//
//  Created by QA on 7/14/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MemBenchNativeIOSViewController;

@interface MemBenchNativeIOSAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet MemBenchNativeIOSViewController *viewController;

@end
