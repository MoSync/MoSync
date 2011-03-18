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
#include <helpers/CPP_IX_WIDGET.h>

@implementation MoSyncUI

NSMutableArray* widgetArray;
NSMutableArray* unusedWidgetHandles;
int currentWidgetIndex = 0;

UIWindow* mainWindow;
UIViewController *mainController;

//bool nativeUIEnabled = false;
static IWidget* sOldScreen = nil;

- (IWidget*)getCurrentlyShownScreen {
	return sOldScreen;
}

- (void)showMoSyncCanvas {
	if(!sOldScreen) return;
	[[sOldScreen getView] removeFromSuperview];
	[mainWindow insertSubview:mainController.view atIndex:0];
	[mainWindow makeKeyAndVisible];
	[mainController.view setNeedsDisplay];
	sOldScreen = nil;
	//nativeUIEnabled = false;
}

- (IWidget*)getWidget: (int) handle {
	IWidget *widget = nil;
	if(handle<0 || handle>=[widgetArray count]) return NULL;
	widget = [widgetArray objectAtIndex:(NSUInteger)handle];
	return widget;
}

- (id)initWithWindow: (UIWindow*) window andController: (UIViewController*)controller {
	[super init];
	widgetArray = [[NSMutableArray alloc] init];
	unusedWidgetHandles = [[NSMutableArray alloc] init];
	
	if(!window) {
		window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];		
		[window makeKeyAndVisible];
	}
		
	mainWindow = window;
	mainController = controller;
	mainWindow.backgroundColor = [UIColor whiteColor];
	
	ScreenWidget* mosyncScreen = [[ScreenWidget alloc] initWithController:mainController];
	[widgetArray addObject:mosyncScreen];
	[mosyncScreen setWidgetHandle:0]; // MAW_CONSTANT_MOSYNC_SCREEN_HANDLE	
	sOldScreen = mosyncScreen;
	
	return self;
}

- (void) close {
	[widgetArray dealloc];
	[unusedWidgetHandles dealloc];
}

- (int) createWidget: (NSString*)name {
	IWidget *created = nil;
	NSString* realName = [name stringByAppendingString:@"Widget"];
	Class widgetClass = NSClassFromString(realName);
	if(widgetClass != nil) {
		created = [[widgetClass alloc] init];
		
		if(widgetClass == [IWidget class]) 
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
	
	[created setWidgetHandle:ret];	
	
	return ret;
}

- (int) destroyWidgetInstance:(IWidget*)widget {
	int handle = [widget getWidgetHandle];	
	[widgetArray replaceObjectAtIndex:handle withObject:[NSNull null]];
	
	int ret;
	int removeRet = [widget remove];
	if(removeRet<0)
		ret = removeRet;
	else
		ret = MAW_RES_OK;

	
	//[widget dealloc];
	[widget release];
	[unusedWidgetHandles addObject:[[NSNumber alloc] initWithInt:handle]];

	return ret;
}

- (void) setPropertyOf: (IWidget*) widget withKey: (NSString*)key toValue: (NSString*)value {	
	[widget setPropertyWithKey:key toValue:value];
}

- (int)show: (IWidget*) widget {
//	if(!nativeUIEnabled) {
//		if(mainController)
//			[mainController.view removeFromSuperview];
//		nativeUIEnabled = true;
	//}
	
	if(sOldScreen != nil) {
		UIView* actualView = [sOldScreen getView];
		[actualView removeFromSuperview];
	} else {
	/*
		if(mainController)
			[mainController.view removeFromSuperview];
	 */
		return MAW_RES_ERROR;
		
		//nativeUIEnabled = true;
	}
	
	[mainWindow insertSubview:[widget getView] atIndex:0];
	
	[widget layout];
	[widget show];
	[mainWindow makeKeyAndVisible];
	//[mainWindow setNeedsDisplay];
	sOldScreen = widget;
	
	return MAW_RES_OK;
}

@end
