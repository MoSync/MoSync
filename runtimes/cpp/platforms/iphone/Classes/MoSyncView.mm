/* Copyright (C) 2009 Mobile Sorcery AB
 
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

#import "MoSyncView.h"
#include "MosyncMain.h"

//#include "iphone_helpers.h"
//#include "Platform.h"

@interface UIApplication(MyExtras) 
- (void)terminateWithSuccess; 
@end

@interface MessageBoxHandler : UIViewController <UIAlertViewDelegate> {
	BOOL kill;
	NSString *msg;
}

@property BOOL kill;
@property (copy, nonatomic) NSString* msg;

- (void)alertViewCancel:(UIAlertView *)alertView;

@end

@implementation MessageBoxHandler

@synthesize kill;
@synthesize msg;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if(kill)
		MoSync_Exit();
}

- (void)alertViewCancel:(UIAlertView *)alertView {
	// don't know if this is allowed...
	if(kill)
		MoSync_Exit();
}

@end

@implementation MoSyncView

- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:YES];
}

-(void) startUpdatingLocation {
	[locationController.locationManager startUpdatingLocation];
}

-(void) stopUpdatingLocation {
	[locationController.locationManager stopUpdatingLocation];
}

- (id)initWithFrame:(CGRect)frame {

	int statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
	frame.origin.y -= statusBarHeight;

	[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent animated:NO];
	[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
	
	
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		self.clearsContextBeforeDrawing = NO;
		//[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent animated:NO];
		//[[UIApplication sharedApplication] setStatusBarHidden:YES animated:YES];
		
		UIDevice* myDevice = [UIDevice currentDevice];
		[myDevice beginGeneratingDeviceOrientationNotifications];
		[myDevice setBatteryMonitoringEnabled:YES];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationChanged:)
													 name:UIDeviceOrientationDidChangeNotification object:nil];
		
		self.frame.origin.y = 0;
		self.frame.origin.x = 0;
		mosyncView = nil;
        // Initialization code
		CGRect appFrame = [[UIScreen mainScreen] bounds];
		CGFloat screenHeight = appFrame.size.height;
		CGFloat screenWidth = appFrame.size.width;
		
		locationController = [[MoSyncCLController alloc] init];
		
		//MoSyncMain(self.frame.size.width, screenHeight, self);
		MoSync_Main(screenWidth, screenHeight, self);
    }
    return self;
}

/*
- (id)initWithCoder:(NSCoder *)decoder {
    if (self = [super initWithCoder:decoder]) {
		[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];

		UIDevice* myDevice = [UIDevice currentDevice];
		[myDevice beginGeneratingDeviceOrientationNotifications];
		[myDevice setBatteryMonitoringEnabled:YES];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationChanged:)
													 name:UIDeviceOrientationDidChangeNotification object:nil];
		
		self.frame.origin.y = 0;
		mosyncView = nil;
        // Initialization code
		CGRect appFrame = [[UIScreen mainScreen] applicationFrame];
		CGFloat screenHeight = appFrame.size.height;
		
		locationController = [[MoSyncCLController alloc] init];
		
<<<<<<< .mine
		MoSync_Main(self.frame.size.width, screenHeight, self);
=======
		//MoSyncMain(self.frame.size.width, screenHeight, self);
		MoSyncMain(320.0, screenHeight, self);
>>>>>>> .r1287
    }
    return self;
}
*/
/*
- (void)mTimerProcess{
	DoneUpdatingMoSyncView();
} 
*/

- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;

    CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSetAllowsAntialiasing(context, false);

	CGContextTranslateCTM(context, 0, CGImageGetHeight(mosyncView));
	CGContextScaleCTM(context, 1.0, -1.0);
	
    CGContextDrawImage(context, rect, mosyncView);	
	MoSync_DoneUpdatingView();
	/*
	NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.000001f
													  target:self
													selector:@selector(mTimerProcess)
													userInfo:nil
													 repeats:NO];
	[timer fire];
	*/
	 
}


- (void)dealloc {
    [super dealloc];
    [locationController release];	
}

bool down = false;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];

	if(!down) {
//		Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_PRESSED);
		MoSync_AddTouchPressedEvent(point.x, point.y);		
		down = true;
	}
}

- (void)deviceOrientationChanged:(NSNotification *)notification {
	UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
	//Base::gEventQueue.addScreenChangedEvent();
	MoSync_AddScreenChangedEvent();
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		//Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_DRAGGED);
		MoSync_AddTouchMovedEvent(point.x, point.y);
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	NSSet *allTouches = [event allTouches];
	UITouch *touch = [[allTouches allObjects] objectAtIndex:0];
	CGPoint point = [touch locationInView:self];
	
	if(down) {
		//Base::gEventQueue.addPointerEvent(point.x, point.y, EVENT_TYPE_POINTER_RELEASED);	
		MoSync_AddTouchReleasedEvent(point.x, point.y);
		down = false;
	}	
}

-(void) messageBox:(id) obj {
	MessageBoxHandler *mbh = (MessageBoxHandler*) obj;
	UIAlertView *alert = [[UIAlertView alloc] 
                          initWithTitle:nil
                          message:mbh.msg
                          delegate:mbh
                          cancelButtonTitle:@"OK"
                          otherButtonTitles:nil];
	
    [alert show];
    [alert release];
}
 
-(void) showMessageBox: (NSString*)msg shouldKill: (bool)kill {
	MessageBoxHandler *mbh = [MessageBoxHandler alloc];
	mbh.kill = kill;
	mbh.msg = msg;
	[self performSelectorOnMainThread: @ selector(messageBox:) withObject:(id)mbh waitUntilDone:NO];
}

@end
