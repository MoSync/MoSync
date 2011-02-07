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

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif

@implementation CheckBoxWidget

- (id)init {
	UISwitch* checkBox = [[[UISwitch alloc] initWithFrame:CGRectMake(0, 0, 100, 60)] retain];
	view = checkBox;

	[view addTarget:self action:@selector(checkBoxPressed) forControlEvents:UIControlEventTouchUpInside];
	
	return [super init];
}

-(void)checkBoxPressed {
	NSLog(@"CheckBox pressed!");
#ifndef NATIVE_TEST
	UISwitch *checkBox = (UISwitch*) view;
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_CLICKED;
	eventData->widgetHandle = handle;
	eventData->checked = checkBox.on;
	event.data = eventData;
	Base::gEventQueue.put(event);
#endif	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"checked"]) {
		bool checked = [value boolValue];
		UISwitch* checkBox = (UISwitch*) view;
		checkBox.on = checked;
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
