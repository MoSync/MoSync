/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	else if([key isEqualToString:@MAW_SCREEN_ICON])
    {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_PROPERTY_VALUE;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		[_controller.tabBarItem setImage:[UIImage imageWithCGImage:imageResource->image]];
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
