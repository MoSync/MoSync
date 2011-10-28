//
//  ___PROJECTNAMEASIDENTIFIER___AppDelegate.m
//  ___PROJECTNAME___
//
//  Created by ___FULLUSERNAME___ on ___DATE___.
//  Copyright ___ORGANIZATIONNAME___ ___YEAR___. All rights reserved.
//

#import "AppDelegate.h"
#import "GLView.h"
#import "StatsView.h"

@implementation AppDelegate

@synthesize window;
@synthesize glView;
@synthesize statsView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
    //allocate the view
    self.statsView = [StatsView alloc];
    
	glView.animationInterval = 1.0 / kRenderingFrequency;
	[glView startAnimation];
    
    //add the view controller's view to the window
    
    [window addSubview:self.statsView];
    
    //[window makeKeyAndVisible];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / kInactiveRenderingFrequency;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}

- (void)dealloc {
	[window release];
	[glView release];
    [statsView release];
	[super dealloc];
}

@end
