//
//  STROPBenchIOSAppDelegate.h
//  STROPBenchIOS
//
//  Created by QA on 7/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class STROPBenchIOSViewController;

@interface STROPBenchIOSAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet STROPBenchIOSViewController *viewController;

@end
