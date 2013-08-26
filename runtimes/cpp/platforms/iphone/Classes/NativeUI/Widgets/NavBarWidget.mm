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

#import "NavBarWidget.h"
#import "UIColorExpanded.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation NavBarWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UINavigationBar* navbar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, 100, 44)];
        navbar.delegate = self;
        prevNavitem = [[UINavigationItem alloc] init];
        currNavitem = [[UINavigationItem alloc] init];
        [navbar pushNavigationItem:prevNavitem animated:false];
        [navbar pushNavigationItem:currNavitem animated:false];
        self.view = navbar;

        self.autoSizeWidth = WidgetAutoSizeFillParent;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        [navbar release];
        navbar = nil;
    }
	return self;
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
	if([key isEqualToString:@MAW_NAV_BAR_TITLE])
    {
		currNavitem.title = value;
	}
	else if([key isEqualToString:@MAW_NAV_BAR_BACK_BTN])
    {
		prevNavitem.title = value;
	}
    else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
        // The background color property must be handled different.
        // For the rest of the widgets the property is handled by
        // the super class(IWidget).
        UINavigationBar* navBar = (UINavigationBar*) self.view;
        UIColor* color = [UIColor colorWithHexString:value];
		if (!color)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
		navBar.tintColor = color;
	}
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPopItem:(UINavigationItem *)item
{
}

- (void)navigationBar:(UINavigationBar *)navigationBar didPushItem:(UINavigationItem *)item
{
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPopItem:(UINavigationItem *)item
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = self.handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
	return false;
}

- (BOOL)navigationBar:(UINavigationBar *)navigationBar shouldPushItem:(UINavigationItem *)item
{
	return YES;
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    [currNavitem release];
    [prevNavitem release];
    [super dealloc];
}

@end
