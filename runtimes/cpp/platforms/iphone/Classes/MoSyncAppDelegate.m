//
//  MoSyncAppDelegate.m
//  MoSync
//
//  Created by Niklas Nummelin on 2/23/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "MoSyncAppDelegate.h"
#import "MoSyncViewController.h"

@implementation MoSyncAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
