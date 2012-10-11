//
//  BenchmarkSuiteAppDelegate.h
//  BenchmarkSuite
//
//  Created by Alexander Samuelsson on 8/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BenchmarkSuiteViewController;

@interface BenchmarkSuiteAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet BenchmarkSuiteViewController *viewController;

@end
