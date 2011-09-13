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

- (id)init {
	UISwitch* checkBox = [[UISwitch alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
	view = checkBox;
	[checkBox addTarget:self action:@selector(checkBoxPressed) forControlEvents:UIControlEventTouchUpInside];
	return [super init];
}

-(void)checkBoxPressed {
	NSLog(@"CheckBox pressed!");
	UISwitch *checkBox = (UISwitch*) view;
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->checked = checkBox.on;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@MAW_CHECK_BOX_CHECKED]) {
		bool checked = [value boolValue];
		UISwitch* checkBox = (UISwitch*) view;
		checkBox.on = checked;
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	if([key isEqualToString:@MAW_CHECK_BOX_CHECKED]) {
		UISwitch* checkBox = (UISwitch*) view;
		return [[NSString alloc] initWithString:(checkBox.on?@"true":@"false")];
	} else {	
		return [super getPropertyWithKey:key];
	}
}

@end
