//
//  CheckBoxWidget.mm
//  nativeuitest
//
//  Created by Magnus Hult <magnus@magnushult.se> on 2011-01-20.
//  Copyright 2010 MoSync AB. All rights reserved.
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
