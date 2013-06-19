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
void MoSync_AddFocusLostEvent();
void MoSync_AddFocusGainedEvent();
void MoSync_Main(int width, int height, MoSyncView* mosyncView);
void MoSync_DidReceiveLocalNotification(UILocalNotification* localNotification);
void MoSync_DidReceivePushNotification(NSDictionary* pushNotification);
void MoSync_ApplicationRegistration(NSInteger errorCode, NSString* text);


#ifdef MOSYNC_NATIVE
#define max_data 6572

extern "C" int resource_selector();
extern "C" unsigned char* InitCustomEventPointer(int mallocSize);
extern "C" int MAMain();

void cpp_main()
{
    InitCustomEventPointer(4194304);
    resource_selector();
    MAMain();
}
#endif

@implementation MoSyncAppDelegate

//@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
 	// Hide status bar HERE because later the space under the status bar is
	// not usable for drawing, etc.
	[[UIApplication sharedApplication] setStatusBarHidden:YES 
                                            withAnimation:UIStatusBarAnimationFade];
    // Override point for customization after app launch    
    viewController = [[MoSyncViewController alloc] init];
	initMoSyncUISyscalls(NULL, viewController);

    int w, h;
    getScreenResolution(w, h);
	MoSync_Main(w, h, (MoSyncView*)[viewController getMoSyncView]);
    
    return YES;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	MoSync_AddFocusLostEvent();
    return;
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	MoSync_AddFocusGainedEvent();
    return;
}

/**
 * Called when the application receives a local notification.
 * @param application The application that received the local notification.
 * @param notification A local notification that encapsulates details about
 * the notification, potentially including custom data.
 */
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    MoSync_DidReceiveLocalNotification(notification);
}

/**
 * Called when the application successfully registers with Apple Push Service (APS).
 * @param application The application that initiated the remote-notification registration process.
 * @param deviceToken A token that identifies the device to APS. The token is an opaque data type
 * because that is the form that the provider needs to submit to the APS servers when it sends a
 * notification to a device. The APS servers require a binary format for performance reasons.
 */
- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSString *parsedToken = [NSString stringWithFormat:@"%@",deviceToken];
    // Remove empty spaces
//    parsedToken = [parsedToken stringByReplacingOccurrencesOfString:@" " withString:@""];
    // Remove "<" and ">" characters
    parsedToken = [parsedToken stringByReplacingOccurrencesOfString:@"<" withString:@""];
    parsedToken = [parsedToken stringByReplacingOccurrencesOfString:@">" withString:@""];
    MoSync_ApplicationRegistration(
        0,
        parsedToken);
}

/**
 * Called when Apple Push Service cannot successfully complete the registration process.
 * @param application The application that initiated the remote-notification registration process.
 * @param error An NSError object that encapsulates information why registration did not succeed.
 * The application can choose to display this information to the user.
 */
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    MoSync_ApplicationRegistration(
        [error code],
        [error localizedDescription]);
}

/**
 * Called when a running application receives a remote notification.
 * @param application The application that received the remote notification.
 * @param userInfo A dictionary that contains information related to the remote notification,
 * potentially including a badge number for the application icon, an alert sound, an alert
 * message to display to the user, a notification identifier, and custom data.
 */
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    MoSync_DidReceivePushNotification(userInfo);
}

/*
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
 */

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
