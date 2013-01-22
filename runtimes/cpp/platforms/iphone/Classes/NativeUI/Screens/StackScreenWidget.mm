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
#include <base/Syscall.h>

#include "Platform.h"
#import "StackScreenWidget.h"
#import "StackScreenWidgetController.h"
#import "UINavigationControllerExpanded.h"

@implementation StackScreenWidget

/**
 * Init function.
 */
- (id)init
{
	UINavigationController* navigationController = [[[StackScreenWidgetController alloc] init] autorelease];
    self = [super initWithController:navigationController];
    if (self)
    {
        stack = [[NSMutableArray alloc] init];
        navigationController.viewControllers = [NSArray array];
        navigationController.delegate = self;
    }
	return self;
}

/**
 * Pop the top view controller from the navigation stack.
 * Called by the UINavigationController object.
 * For more info see UINavigationControllerExpanded file.
 */
- (void)viewControllerWillBePoped
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_STACK_SCREEN_POPPED;
	eventData->widgetHandle = self.handle;
    int fromScreenHandle = -1;
    int toScreenHandle = -1;

    if ([stack count] > 0)
    {
        int fromScreenIndex = [stack count] - 1;
        IWidget* fromScreen = [stack objectAtIndex:fromScreenIndex];
        fromScreenHandle = fromScreen.handle;
    }
    if ([stack count] > 1)
    {
        int toScreenIndex = [stack count] - 2;
        IWidget* toScreen = [stack objectAtIndex:toScreenIndex];
        toScreenHandle = toScreen.handle;
    }

    eventData->fromScreen = fromScreenHandle;
    eventData->toScreen = toScreenHandle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);

    [stack removeLastObject];
}

/**
 * Push a view controller on the navigation stack.
 * @param child Widget containing a view controller(e.g. ScreenWidget).
 */
- (void)push:(IWidget*)child
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	ScreenWidget* screen = (ScreenWidget*)child;
	[navigationController pushViewController:[screen getController] animated:YES];
	[stack addObject:child];
	float navBarHeight = navigationController.toolbar.bounds.size.height;
	float viewHeight = self.height - navBarHeight;
    child.size = CGSizeMake(self.width, viewHeight);
	[child show];
}

/**
 * Pop the top screen from the stack.
 */
- (void)pop
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	[navigationController popViewControllerAnimated:YES];
}

/**
 * Recalculate its and children size.
 */
- (void)layout
{
	UINavigationController* navigationController = (UINavigationController*)_controller;
	float navBarHeight = navigationController.toolbar.bounds.size.height;
    float viewWidth = self.width;
	float viewHeight = self.height - navBarHeight;
	[self.view setNeedsLayout];
	for (IWidget *child in stack)
    {
        child.size = CGSizeMake(viewWidth, viewHeight);
        [child layout];
	}
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
- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_SCREEN_TITLE])
    {
		_controller.title = value;
	}
	else if([key isEqualToString:@MAW_STACK_SCREEN_BACK_BUTTON_ENABLED])
    {
		UINavigationController* navigationController = (UINavigationController*)_controller;
		navigationController.navigationBar.backItem.hidesBackButton = [value boolValue];
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

/**
 * Dealloc method.
 */
- (void)dealloc
{
    [stack release];
    [super dealloc];
}

@end
