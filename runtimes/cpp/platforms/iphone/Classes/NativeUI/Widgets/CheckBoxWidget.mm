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

#import "CheckBoxWidget.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation CheckBoxWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UISwitch* checkBox = [[UISwitch alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
        self.view = checkBox;
        [checkBox addTarget:self action:@selector(checkBoxPressed) forControlEvents:UIControlEventTouchUpInside];
        [checkBox release];
        checkBox = nil;
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
	if([key isEqualToString:@MAW_CHECK_BOX_CHECKED])
    {
		bool checked = [value boolValue];
		UISwitch* checkBox = (UISwitch*) self.view;
		checkBox.on = checked;
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
	if([key isEqualToString:@MAW_CHECK_BOX_CHECKED])
    {
		UISwitch* checkBox = (UISwitch*) self.view;
		return [[NSString alloc] initWithString:(checkBox.on?kWidgetTrueValue:kWidgetFalseValue)];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Called for a touch-up event in the control where the finger is inside the bounds of the control.
 */
-(void)checkBoxPressed
{
	UISwitch *checkBox = (UISwitch*) self.view;
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = self.handle;
	eventData->checked = checkBox.on;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

@end
