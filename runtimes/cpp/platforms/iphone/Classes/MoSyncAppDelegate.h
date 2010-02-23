//
//  MoSyncAppDelegate.h
//  MoSync
//
//  Created by Niklas Nummelin on 2/23/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MoSyncViewController;

@interface MoSyncAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    MoSyncViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MoSyncViewController *viewController;

@end

