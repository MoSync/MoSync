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

- (IWidget*)getWidget: (int) handle {
	IWidget *widget = nil;
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
	
	[created setWidgetHandle:[widgetArray count]];
	[created wasCreated];
	
	int ret = MAW_RES_ERROR;
	
	if([unusedWidgetHandles count] > 0) {
		ret = [[unusedWidgetHandles objectAtIndex:([unusedWidgetHandles count]-1)] intValue];
		[unusedWidgetHandles removeLastObject];
		[widgetArray replaceObjectAtIndex:ret withObject:created];
	} else {
		[widgetArray addObject:created];
		ret = [widgetArray count]-1;
	}
	
	return ret;
}

- (int) destroyWidgetInstance:(NSNumber*)handleNumber {
	int handle = [handleNumber intValue];
	[handleNumber release];
	
	IWidget* widget = [self getWidget:handle];
	if(!widget) 
		return MAW_RES_INVALID_HANDLE;
	[widget remove];	
	[widget dealloc];
	[widgetArray replaceObjectAtIndex:handle withObject:[NSNull null]];
	[unusedWidgetHandles addObject:[[NSNumber alloc] initWithInt:handle]];

	return MAW_RES_OK;
}

- (void) setPropertyOf: (IWidget*) widget withKey: (NSString*)key toValue: (NSString*)value {	
	[widget setPropertyWithKey:key toValue:value];
}

bool nativeUIEnabled = false;

static IWidget* sOldScreen = nil;


- (int)show: (IWidget*) widget {
	if(!nativeUIEnabled) {
		if(mainController)
			[mainController.view removeFromSuperview];
		nativeUIEnabled = true;
	}
	
	if(sOldScreen != nil) {
		UIView* actualView = [sOldScreen getView];
		[actualView removeFromSuperview];
	}
	
	[mainWindow insertSubview:[widget getView] atIndex:0];
	
	[widget layout];
	[mainWindow makeKeyAndVisible];
	sOldScreen = widget;
	
	return MAW_RES_OK;
}

@end
