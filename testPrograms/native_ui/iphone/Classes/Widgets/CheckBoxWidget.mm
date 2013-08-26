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

//
//  CheckBoxWidget.mm
//  nativeuitest
//
//  Created by Magnus Hult <magnus@magnushult.se> on 2011-01-20.
//

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
