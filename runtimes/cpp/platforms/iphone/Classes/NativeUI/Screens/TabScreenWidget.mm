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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "TabScreenWidget.h"
#import "TabScreenWidgetController.h"
#include "Platform.h"

@implementation TabScreenWidget


-(void)tabBarController:(UITabBarController*)tabBarController didSelectViewController:(UIViewController*)viewController {
	MAHandle controllerHandle = 0;
	NSUInteger index = 0;

	for (UIViewController *child in tabBarController.viewControllers)
    {
		if(child == viewController) {
			if(index >= [_children count]) return;
			controllerHandle = [[_children objectAtIndex:index] handle];
			break;
		}
		index++;
	}

	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_TAB_CHANGED;
	eventData->widgetHandle = self.handle;
	eventData->tabIndex = index;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

/**
 * Init function.
 */
- (id)init
{
	UITabBarController* tabBarController = [[[TabScreenWidgetController alloc] init] autorelease];
	tabBarController.viewControllers = [NSArray array];
	tabBarController.delegate = self;
	return [super initWithController:tabBarController];
}

/**
 * Adds an screen widget to the tab screen.
 * @param child Screen to be added.
 * @return One of the following constants:
 * - MAW_RES_OK if the screen was added.
 * - MAW_RES_ERROR if the screen param is not ScreenWidget or derived from it.
 */
- (int)addChild:(IWidget*)screen
{
    if ([screen class] != [ScreenWidget class] &&
        [screen superclass] != [ScreenWidget class])
    {
        return MAW_RES_ERROR;
    }

	UITabBarController* tabBarController = (UITabBarController*)_controller;
	ScreenWidget* screenWidget = (ScreenWidget*)screen;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems addObject:[screenWidget getController]];
	tabBarController.viewControllers = newItems;

	[super addChild:screen toSubview:NO];
    return MAW_RES_OK;
}

/**
 * Insert a screen to a given index.
 * @param screen Widget to be added.
 * @param index The index where the screen should be inserted.
 * - MAW_RES_OK if the screen was added.
 * - MAW_RES_INVALID_INDEX if the index param is invalid.
 * - MAW_RES_ERROR if the screen param is not ScreenWidget or derived from it.
 */
- (int)insertChild: (IWidget*)screen atIndex:(NSNumber*)index
{
    if ([screen class] != [ScreenWidget class] &&
        [screen superclass] != [ScreenWidget class])
    {
        return MAW_RES_ERROR;
    }

	int ret = [super insertChild:screen atIndex:index toSubview:NO];
	if (ret != MAW_RES_OK)
    {
		return ret;
    }

	UITabBarController* tabBarController = (UITabBarController*)_controller;
	ScreenWidget* screenWidget = (ScreenWidget*)screen;
	NSMutableArray *newItems = [NSMutableArray arrayWithArray:tabBarController.viewControllers];
	[newItems insertObject:[screenWidget getController] atIndex:[index intValue]];
	tabBarController.viewControllers = newItems;

	return MAW_RES_OK;
}

/**
 * Remove a screen from the tab screen.
 * @param screen Screen to be removed.
 */
- (void)removeChild: (IWidget*)screen
{
	[super removeChild:screen fromSuperview:NO];
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_SCREEN_TITLE])
    {
		_controller.title = value;
	}
	else if([key isEqualToString:@MAW_TAB_SCREEN_CURRENT_TAB])
    {
		unsigned int index = [value intValue];
		UITabBarController* tabBarController = (UITabBarController*)_controller;
		if(index >= [tabBarController.viewControllers count]) return MAW_RES_INVALID_INDEX;
		tabBarController.selectedViewController = [tabBarController.viewControllers objectAtIndex:index];
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if([key isEqualToString:@MAW_TAB_SCREEN_CURRENT_TAB])
    {
		UITabBarController* tabBarController = (UITabBarController*)_controller;
		return [[[NSNumber numberWithInt: tabBarController.selectedIndex] stringValue] retain];
	}
	else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Recalculate its and children size.
 */

- (void)layout
{
	UITabBarController* tabBarController = (UITabBarController*)_controller;
	float tabBarHeight = tabBarController.tabBar.bounds.size.height;
	float viewWidth = self.width;
	float viewHeight = self.height - tabBarHeight;
    CGRect tabBarRect = CGRectMake(self.originX,
                                   viewHeight,
                                   viewWidth,
                                   tabBarHeight);
    tabBarController.tabBar.frame = tabBarRect;
	[self.view setNeedsLayout];

	for (IWidget *child in _children)
    {
        child.size = CGSizeMake(viewWidth, viewHeight);
        [child layout];
	}
}

@end
