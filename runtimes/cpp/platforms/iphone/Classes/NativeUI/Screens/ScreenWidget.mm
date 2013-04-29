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

#import "ScreenWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "Platform.h"
#include <base/Syscall.h>
#include "MoSyncUI.h"
#include "MoSyncUISyscalls.h"

@implementation ScreenWidget

/**
 * Init function.
 * A view controller will be created for you.
 */
- (id)init
{
	UIViewController* controller = [[[ScreenWidgetController alloc] init] autorelease];
	return [self initWithController:controller];
}

/**
 * Init function.
 * @param controller Controller for the view.
 */
- (id)initWithController:(UIViewController*)controller
{
    self = [super init];
    if (self)
    {
        _controller = [controller retain];
        self.view = _controller.view;
        controller.title = @"";
        self.view.autoresizesSubviews = YES;
    }
	return self;
}

/**
 * Adds an widget to the end of the children list.
 * A ScreenWidget can have only one child.
 * @param child Widget to be added.
 * @return MAW_RES_OK for success, MAW_RES_ERROR if the screen already has an child.
 */
- (int)addChild:(IWidget*)child
{
    if ([_children count] > 0)
    {
        return MAW_RES_ERROR;
    }

    child.isMainWidget = TRUE;

	[super addChild:child toSubview:YES];
    [self layout];
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index Screen widget can have only one child, so this value must be zero.
 * @return One of the following values:
 * - MAW_RES_OK if the child could be added to the parent.
 * - MAW_RES_INVALID_INDEX if the index param is not zero.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index
{
    if (index != 0)
    {
        return MAW_RES_INVALID_INDEX;
    }
    [self addChild:child];
    return MAW_RES_OK;
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child
{
    child.isMainWidget = FALSE;
    [super removeChild:child];
}

/**
 * Set child's size.
 */
-(void) layout
{
    // Check if the screen has child.
    if ([_children count] == 0)
    {
        return;
    }

    IWidget* child = [_children objectAtIndex:0];
    child.size = self.size;
}

/**
 * Get view's controller.
 * @return View's controller.
 */
- (UIViewController*) getController
{
    return _controller;
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
    else if([key isEqualToString:@MAW_SCREEN_SET_BACK_BUTTON])
    {
        MoSyncUI* mosyncUI = getMoSyncUI();
        MAHandle navBarButtonHandle = [value intValue];
        IWidget* navBarButtonWidget = [mosyncUI getWidget:navBarButtonHandle];
        if (!navBarButtonWidget) return MAW_RES_INVALID_HANDLE;
        if ([navBarButtonWidget class] != [NavBarButtonWidget class]) return MAW_RES_ERROR;

        NavBarButtonWidget* navBarButton = (NavBarButtonWidget*)navBarButtonWidget;
        UINavigationController* navigationController = (UINavigationController*)[self getController];
        [navigationController.navigationItem setBackBarButtonItem:navBarButton.barButtonItem];
    }
	else if([key isEqualToString:@MAW_SCREEN_ICON])
    {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		[_controller.tabBarItem setImage:[UIImage imageWithCGImage:imageResource->image]];
	}
    else if([key isEqualToString:@MAW_SCREEN_REMOVE_NAV_BAR_BUTTON_WIDGET])
    {
        MoSyncUI* mosyncUI = getMoSyncUI();
        MAHandle navBarButtonHandle = [value intValue];
        IWidget* navBarButtonWidget = [mosyncUI getWidget:navBarButtonHandle];
        if (!navBarButtonWidget) return MAW_RES_INVALID_HANDLE;
        if ([navBarButtonWidget class] != [NavBarButtonWidget class]) return MAW_RES_ERROR;

        NavBarButtonWidget* navBarButton = (NavBarButtonWidget*)navBarButtonWidget;
        UINavigationController* navigationController = (UINavigationController*)[self getController];
        UINavigationItem* navigationItem = navigationController.navigationItem;

        NSMutableArray* newButtonArrayR = [navigationItem.rightBarButtonItems mutableCopy];
        [newButtonArrayR removeObject:navBarButton.barButtonItem];
        [navigationItem setRightBarButtonItems:newButtonArrayR];

        NSMutableArray* newButtonArrayL = [navigationItem.leftBarButtonItems mutableCopy];
        [newButtonArrayL removeObject:navBarButton.barButtonItem];
        [navigationItem setLeftBarButtonItems:newButtonArrayL];
    }
    else if([key isEqualToString:@MAW_SCREEN_REMOVE_NAV_BAR_BUTTONS])
    {
        UINavigationController* navigationController = (UINavigationController*)[self getController];
        if ([value intValue] == MAW_SCREEN_NAV_BAR_SIDE_RIGHT)
        {
            [navigationController.navigationItem setRightBarButtonItems:[[NSArray alloc] init]];
        }
        else if ([value intValue] == MAW_SCREEN_NAV_BAR_SIDE_LEFT)
        {
            [navigationController.navigationItem setLeftBarButtonItems:[[NSArray alloc] init]];
        }
    }
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (void)attachNavBarButton:(int)side navBarButtonWidget:(NavBarButtonWidget*)button atIndex:(int)index
{
    UIBarButtonItem* buttonItem = button.barButtonItem;

    UINavigationController* navigationController = (UINavigationController*)[self getController];
    NSMutableArray* buttons;
    if (side == MAW_SCREEN_NAV_BAR_SIDE_RIGHT)
        buttons = [navigationController.navigationItem.rightBarButtonItems mutableCopy];
    else
        buttons = [navigationController.navigationItem.leftBarButtonItems mutableCopy];

    if (!buttons) buttons = [[NSMutableArray alloc] init];

    if (index < 0 || index >= [buttons count])
        [buttons addObject:buttonItem];
    else
        [buttons insertObject:buttonItem atIndex:index];

    if (side == MAW_SCREEN_NAV_BAR_SIDE_RIGHT)
        [navigationController.navigationItem setRightBarButtonItems:buttons];
    else
        [navigationController.navigationItem setLeftBarButtonItems:buttons];
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if ([key isEqualToString:@MAW_SCREEN_IS_SHOWN])
	{
		return [self.isShownProperty retain];
	}
	else
	{
		return [super getPropertyWithKey:key];
	}
}

/**
 * Check if a given child screen is shown inside this screen.
 * This function should be implemented by all classed inherited from this class,
 * such as StackScreenWidget and TabScreenWidget.
 * @param childScreen Screen to check.
 * @return This function returns NO.
 */
- (BOOL)isChildScreenShown:(ScreenWidget*)childScreen
{
	return NO;
}

/**
 * Check if the screen is shown.
 * @return "true" is the screen is shown, "false" otherwise.
 */
- (NSString*)isShownProperty
{
	if (!self.parent)
	{
		MoSyncUI* mosyncUI = getMoSyncUI();
		IWidget* currentlyShownScreen = [mosyncUI getCurrentlyShownScreen];
		return ([currentlyShownScreen isEqual:self]) ? kWidgetTrueValue : kWidgetFalseValue;
	}

	if ([self.parent class] == [ScreenWidget class] ||
		[self.parent superclass] == [ScreenWidget class])
	{
		ScreenWidget* parentScreen = (ScreenWidget*)self.parent;
		return [parentScreen isChildScreenShown:self] ? kWidgetTrueValue : kWidgetFalseValue;
	}
	return kWidgetFalseValue;
}

/**
 * Dealloc method.
 */
- (void)dealloc
{
    [_controller release];
    [super dealloc];
}

@end
