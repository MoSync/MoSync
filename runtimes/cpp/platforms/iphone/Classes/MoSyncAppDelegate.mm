/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#import "MoSyncAppDelegate.h"
#import "MoSyncViewController.h"

#include "MoSyncUISyscalls.h"

//#include "Platform.h"
//#import "MoSyncMain.h"

void MoSync_AddCloseEvent();

@implementation MoSyncAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	// Hide status bar HERE because later the space under the status bar is
	// not usable for drawing, etc.
	[[UIApplication sharedApplication] setStatusBarHidden:YES 
										withAnimation:UIStatusBarAnimationFade];
    // Override point for customization after app launch    
	window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    viewController = [[MoSyncViewController alloc] init];
	initMoSyncUISyscalls(window, viewController);
	[window addSubview:viewController.view];
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
//	Base::gEventQueue.addCloseEvent();
	MoSync_AddCloseEvent();
}

- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
