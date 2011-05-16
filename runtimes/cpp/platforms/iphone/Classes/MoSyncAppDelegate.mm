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

@class MoSyncView;


void getScreenResolution(int &w, int &h);
void initMoSyncUISyscalls(UIWindow* window, UIViewController* viewController);
void MoSync_AddCloseEvent();
void MoSync_Main(int width, int height, MoSyncView* mosyncView);

@implementation MoSyncAppDelegate

//@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
	// Hide status bar HERE because later the space under the status bar is
	// not usable for drawing, etc.
	[[UIApplication sharedApplication] setStatusBarHidden:YES 
										withAnimation:UIStatusBarAnimationFade];
	//[[UIApplication sharedApplication] setStatusBarHidden:NO 
	//									withAnimation:UIStatusBarAnimationFade];	
	
    // Override point for customization after app launch    
	//UIWindow* window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    viewController = [[MoSyncViewController alloc] init];
	initMoSyncUISyscalls(NULL, viewController);

	//CGRect appFrame = [[UIScreen mainScreen] bounds];
	//CGFloat screenHeight = appFrame.size.height;
	//CGFloat screenWidth = appFrame.size.width;
	//MoSync_Main(screenWidth, screenHeight, (MoSyncView*)[viewController getMoSyncView]);

    int w, h;
    getScreenResolution(w, h);
	MoSync_Main(w, h, (MoSyncView*)[viewController getMoSyncView]);

	
}

- (void)applicationWillTerminate:(UIApplication *)application {
//	Base::gEventQueue.addCloseEvent();
	MoSync_AddCloseEvent();
}

- (void)dealloc {
    [viewController release];
  //  [window release];
    [super dealloc];
}


@end
