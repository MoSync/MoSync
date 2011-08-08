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

#import "TabScreenWidget.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation TabScreenWidget


-(void)tabBarController:(UITabBarController*)tabBarController didSelectViewController:(UIViewController*)viewController {
	MAHandle controllerHandle = 0;
	NSUInteger index = 0;
	
	for (UIViewController *child in tabBarController.viewControllers)
    {
		if(child == viewController) {
			if(index >= [children count]) return;
			controllerHandle = [[children objectAtIndex:index] getWidgetHandle];
			break;
		}
		index++;
	}

	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_TAB_CHANGED;
	eventData->widgetHandle = handle;
	eventData->tabIndex = index;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);	
	
	//if(index < [children count])
	//	[[children objectAtIndex:index] layout];
}

- (id)init {
    //view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	UITabBarController* tabBarController = [[[UITabBarController alloc] init] retain];
	//controller = tabBarController;
	tabBarController.viewControllers = [NSArray array];	
	//view = controller.view;
	//controller.view = view;
	tabBarController.delegate = self;
	
	return [super initWithController:tabBarController];	
}

- (void)addChild: (IWidget*)child {
	UITabBarController* tabBarController = (UITabBarController*)controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems addObject:[screen getController]];
	tabBarController.viewControllers = newItems;
	
	//[super addChild:child];
	[super addChild:child toSubview:NO];
	
	//UIView *childView = [screen getView];	
	//[childView setFrame: view.frame];

	//view.autoresizesSubviews = YES;	
	//[view setAutoresizingMask:UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];

}

- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index {
	int ret = [super insertChild:child atIndex:index toSubview:NO];
	if(ret!=MAW_RES_OK)
		return ret;
	
	UITabBarController* tabBarController = (UITabBarController*)controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems insertObject:[screen getController] atIndex:[index intValue]];
	tabBarController.viewControllers = newItems;
	
	return MAW_RES_OK;

}

- (void)removeChild: (IWidget*)child {
	[super removeChild:child fromSuperview:NO];	
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"title"]) {
		controller.title = value;
	} 
	else if([key isEqualToString:@"currentTab"]) {
		unsigned int index = [value intValue];
		UITabBarController* tabBarController = (UITabBarController*)controller;
		if(index >= [tabBarController.viewControllers count]) return MAW_RES_INVALID_INDEX;
		tabBarController.selectedViewController = [tabBarController.viewControllers objectAtIndex:index]; 
		//tabBarController.selectedIndex = [value intValue];
		//[tabBarController.selectedViewController viewDidAppear:YES];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@"currentTab"]) {
		UITabBarController* tabBarController = (UITabBarController*)controller;
		return [[NSNumber numberWithInt: tabBarController.selectedIndex] stringValue];
	}
	else {
		return [super getPropertyWithKey:key];
	}
}

- (UIViewController*) getController {
	return controller;
}

- (void)layout {
	UITabBarController* tabBarController = (UITabBarController*)controller;
	
	int tabBarHeight = tabBarController.tabBar.bounds.size.height;
	int viewWidth = view.frame.size.width; 
	int viewHeight = view.frame.size.height - tabBarHeight; 
	
	
	[view setNeedsLayout];
	//[view setNeedsDisplay]
	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		[childView setFrame:CGRectMake(0, 0, viewWidth, viewHeight)];		
		
		[child layout];
		
	}
}

@end
