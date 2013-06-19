/* Copyright (C) 2011 MoSync AB

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

#import "MoSyncUI.h"
#import "LabelWidget.h"
#import "ListViewWidget.h"
#import "ScreenWidget.h"
#import "ReflectionWidget.h"
#import "UIKit/UIPopoverController.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "TouchHelper.h"
#include "MoSyncMain.h"
#include "MoSyncViewController.h"
#import "ScreenOrientation.h"
#import "MoSyncUISyscalls.h"
#import "WidgetUtils.h"

@interface MoSyncUIWindow : UIWindow {
	TouchHelper* touchHelper;
}

- (id)initWithFrame:(CGRect)rect;
- (void) handleTouchBegan:(UITouch *)touch;
- (void) handleTouchMoved:(UITouch *)touch;
- (void) handleTouchEnded:(UITouch *)touch;
- (void) handleTouchCancelled:(UITouch *)touch;
- (CGPoint) pointUsingOrientation:(CGPoint) originalPoint;
@end

@implementation MoSyncUIWindow

- (id)initWithFrame:(CGRect)rect {
	touchHelper = [[TouchHelper alloc] init];
	self.multipleTouchEnabled = YES;
	return [super initWithFrame:rect];
}

- (CGFloat) getScreenScale {
    CGFloat scale = 1.0f;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) {
        scale = [UIScreen mainScreen].scale;
    }
    return scale;
}

- (void) handleTouchBegan:(UITouch *)touch
{
	CGFloat screenScale = [self getScreenScale];
    CGPoint point = [touch locationInView:self];
    point = [self pointUsingOrientation:point];
    int touchId = [touchHelper addTouch: touch];
    MoSync_AddTouchPressedEvent(point.x*screenScale, point.y*screenScale, touchId);
}

- (void) handleTouchMoved:(UITouch *)touch
{
	CGFloat screenScale = [self getScreenScale];
    CGPoint point = [touch locationInView:self];
    point = [self pointUsingOrientation:point];
    int touchId = [touchHelper getTouchId: touch];
    MoSync_AddTouchMovedEvent(point.x*screenScale, point.y*screenScale, touchId);
}

- (void) handleTouchEnded:(UITouch *)touch
{
	CGFloat screenScale = [self getScreenScale];
    CGPoint point = [touch locationInView:self];
    int touchId = [touchHelper getTouchId: touch];
    point = [self pointUsingOrientation:point];
    MoSync_AddTouchReleasedEvent(point.x*screenScale, point.y*screenScale, touchId);
    [touchHelper removeTouch: touch];
}

- (void) handleTouchCancelled:(UITouch *)touch
{
    CGPoint point = [touch locationInView:self];
    point = [self pointUsingOrientation:point];
    int touchId = [touchHelper getTouchId: touch];
    MoSync_AddTouchReleasedEvent(point.x, point.y, touchId);
    [touchHelper removeTouch: touch];
}

- (void)sendEvent:(UIEvent *)event
{
    [super sendEvent:event];

    // Fix for MOSYNC-1593 Swipe Jesture doesn't work on images screen on NativeUIDemo.
    // On iOS 5 touchesMoved, touchesEnded and touchesCancelled methods are not called.
    // So we listen for all UIWindow related events, but handle only the touch related events.
    UIEventType eventType = event.type;
    if (eventType == UIEventTypeTouches)
    {
        NSSet* touches = [event allTouches];
        for (UITouch* touch in [touches allObjects])
        {
            if(touch.phase ==  UITouchPhaseBegan)
            {
                [self handleTouchBegan:touch];
            }
            else if(touch.phase == UITouchPhaseMoved)
            {
                [self handleTouchMoved:touch];
            }
            else if(touch.phase == UITouchPhaseCancelled)
            {
                [self handleTouchCancelled:touch];
            }
            else if(touch.phase == UITouchPhaseEnded)
            {
                [self handleTouchEnded:touch];
            }
        }
    }
}

- (CGPoint) pointUsingOrientation:(CGPoint) originalPoint
{
    CGPoint returnedPoint = CGPointMake(0, 0);
    int orientation = [[ScreenOrientation getInstance] getCurrentScreenOrientation];
    MoSyncUI* mosyncUI = getMoSyncUI();
    IWidget* currentScreen = [mosyncUI getCurrentlyShownScreen];
    int height = [[currentScreen view] frame].size.height;
    int width = [[currentScreen view] frame].size.width;
    switch (orientation)
    {
        case MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN:
            returnedPoint.x = width - originalPoint.x;
            returnedPoint.y = height - originalPoint.y;
            break;
        case MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT:
            returnedPoint.x = originalPoint.y;
            returnedPoint.y = width - originalPoint.x;
            break;
        case MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT:
            returnedPoint.x = height - originalPoint.y;
            returnedPoint.y = originalPoint.x;
            break;
        case MA_SCREEN_ORIENTATION_PORTRAIT:
        default:
            returnedPoint.x = originalPoint.x;
            returnedPoint.y = originalPoint.y;
            break;
    }
    return returnedPoint;
}

@end


@implementation MoSyncUI

NSMutableArray* widgetArray;
NSMutableArray* unusedWidgetHandles;
int currentWidgetIndex = 0;

UIWindow* mainWindow;
UIViewController *mainController;
UIPopoverController *popOverController; //For iPads

static IWidget* sOldScreen = nil;

- (IWidget*)getCurrentlyShownScreen {
	return sOldScreen;
}

- (IWidget*)getWidget: (int) handle {
	IWidget *widget = nil;
	if(handle<0 || handle>=[widgetArray count]) return NULL;
	widget = [widgetArray objectAtIndex:(NSUInteger)handle];
    if([NSNull null]==(id)widget)
        return NULL;
	return widget;
}

- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller {
	[super init];
	widgetArray = [[NSMutableArray alloc] init];
	unusedWidgetHandles = [[NSMutableArray alloc] init];

	if(!window) {
		window = [[MoSyncUIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
		[window makeKeyAndVisible];
	}

    if(!controller) {
        controller = [[MoSyncViewController alloc] init];
    }

	mainWindow = window;
	mainController = controller;
	mainWindow.backgroundColor = [UIColor whiteColor];

	ScreenWidget* mosyncScreen = [[ScreenWidget alloc] initWithController:mainController];
	[widgetArray addObject:mosyncScreen];
	[mosyncScreen setHandle:0]; // MAW_CONSTANT_MOSYNC_SCREEN_HANDLE

	sOldScreen = nil; // show will set this after the screen is shown.
	[self show: mosyncScreen];
	return self;
}

- (void) close {
	[widgetArray dealloc];
	[unusedWidgetHandles dealloc];
	if (popOverController != nil) {
		[popOverController release];
		popOverController = nil;
	}
}

- (int) createWidget: (NSString*)name {
	IWidget *created = nil;
	NSString* realName = [name stringByAppendingString:@"Widget"];
	Class widgetClass = NSClassFromString(realName);
    if(widgetClass == [IWidget class]) {
        return MAW_RES_ERROR;
    }

	if(widgetClass != nil) {
		created = [[widgetClass alloc] init];
        if(created == nil)
            return MAW_RES_ERROR;

	} else {
		//created = [[ReflectionWidget alloc] initWithName:name];
		return MAW_RES_INVALID_TYPE_NAME;
	}

	// todo handle these things.
	if(created == nil) return MAW_RES_INVALID_TYPE_NAME;

	int ret = MAW_RES_ERROR;

	if([unusedWidgetHandles count] > 0) {
		ret = [[unusedWidgetHandles objectAtIndex:([unusedWidgetHandles count]-1)] intValue];
		[unusedWidgetHandles removeLastObject];
		[widgetArray replaceObjectAtIndex:ret withObject:created];
	} else {
		[widgetArray addObject:created];
		ret = [widgetArray count]-1;
	}

	[created setHandle:ret];
    [created release];
	return ret;
}

- (int) destroyWidgetInstance:(IWidget*)widget {
	int handle = widget.handle;

    if(widget == sOldScreen) {
        UIView* actualView = [sOldScreen view];
		[actualView removeFromSuperview];
        sOldScreen = nil;
    }

	int ret;
	int removeRet = [widget remove];
	if(removeRet<0)
		ret = removeRet;
	else
		ret = MAW_RES_OK;

    // The array will release the widget.
    [widgetArray replaceObjectAtIndex:handle withObject:[NSNull null]];

    NSNumber* numHandle = [[NSNumber alloc] initWithInt:handle];
    [unusedWidgetHandles addObject:numHandle];
    [numHandle release];

	return ret;
}

- (void) setPropertyOf: (IWidget*) widget withKey: (NSString*)key toValue: (NSString*)value {
	[widget setPropertyWithKey:key toValue:value];
}

- (int)show: (IWidget*) widget {
    if(sOldScreen == widget)
        return MAW_RES_OK;

    if(sOldScreen != nil) {
        if ([widget class] != [ScreenWidget class] &&
            [[widget superclass] class] != [ScreenWidget class])
            {
                return MAW_RES_INVALID_SCREEN;
            }
    }

	if(sOldScreen != nil) {
		UIView* actualView = [sOldScreen view];
		[actualView removeFromSuperview];
	}

    ScreenWidget* screen = (ScreenWidget*)widget;
    mainWindow.rootViewController = [screen getController];
	//[mainWindow insertSubview:[widget view] atIndex:0];

	[widget layout];
	[widget show];
	[mainWindow makeKeyAndVisible];
	sOldScreen = widget;

	return MAW_RES_OK;
}

// Shows a screen with a transition.
- (int)show: (IWidget*) widget withTransitionType: (NSNumber*) transitionType
                            andTransitionDuration: (NSNumber*) transitionDuration {
    // When refactoring consider adding a new error code if show is called
    // on the same screen and create a validation function for the two cases below.
    if(sOldScreen == widget)
        return MAW_RES_OK;

    if(sOldScreen != nil) {
        if ([widget class] != [ScreenWidget class] &&
            [[widget superclass] class] != [ScreenWidget class])
        {
            return MAW_RES_INVALID_SCREEN;
        }
    }

	if(sOldScreen != nil) {
		UIView* actualView = [sOldScreen view];
		[actualView removeFromSuperview];
	}

    ScreenWidget* screen = (ScreenWidget*)widget;

    int screenTransitionResult = [ScreenTransitionsUtils doScreenTransition:mainWindow
                                                                   toScreen:screen
                                                         withTransitionType:transitionType
                                                      andTransitionDuration:transitionDuration];

    // Case1: If the screen transition type is not available on iOS do show without
    // screen transition and return corresponding code error.
    // Case2: If screen transition duration is invalid do normal show and return error code.
    if ( MAW_RES_INVALID_SCREEN_TRANSITION_TYPE == screenTransitionResult ||
         MAW_RES_INVALID_SCREEN_TRANSITION_DURATION == screenTransitionResult)
    {
        int showResult = [self show:widget];
        if ( MAW_RES_OK == showResult )
        {
            return screenTransitionResult;
        }
        return showResult;
    }

	[widget layout];
	[widget show];
	[mainWindow makeKeyAndVisible];
	sOldScreen = widget;

	return MAW_RES_OK;
}

- (void) showModal: (UIViewController*) controller {
	[self performSelectorOnMainThread:@selector(showModalHelper:)
						   withObject:controller
						waitUntilDone:NO];

}

- (void) showModalHelper: (UINavigationController*) controller {

	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
		[[(ScreenWidget*)sOldScreen getController] presentModalViewController:controller animated:YES];
	}
	else {
		if(popOverController == nil)
		{
			popOverController = [[UIPopoverController alloc] initWithContentViewController:controller];
		}
		popOverController.contentViewController = controller;

        // Create rect's anchor. It must not have the same size as the device's screen.
        CGRect rect = CGRectMake(controller.view.frame.origin.x,
                                 controller.view.frame.origin.y,
                                 1,
                                 1);
		[popOverController presentPopoverFromRect:rect
                                           inView:[sOldScreen view]
                         permittedArrowDirections:UIPopoverArrowDirectionAny
                                         animated:YES];
	}

	[[UIApplication sharedApplication] setStatusBarHidden:YES
                                            withAnimation:UIStatusBarAnimationFade];
}

- (void) hideModal {
	if (UIUserInterfaceIdiomPhone == UI_USER_INTERFACE_IDIOM()) {
		[[(ScreenWidget*)sOldScreen getController] dismissModalViewControllerAnimated:NO];
	}
	else {
		if(popOverController != nil)
		{
			[popOverController dismissPopoverAnimated:NO];
		}
	}

}
@end
