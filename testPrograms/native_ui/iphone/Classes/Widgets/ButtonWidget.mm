//
//  ButtonWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ButtonWidget.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#endif

@implementation ButtonWidget

- (id)init {
	view = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain]; // TODO: do have to do this (retain)??
	//view.frame = CGRectMake(0, 0, 100, 40);
//	view = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 30, 30)];
	
	[view addTarget:self action:@selector(buttonPressed) forControlEvents:UIControlEventTouchUpInside];
	
	return [super init];
}

-(void)buttonPressed {
	NSLog(@"Button pressed!");
#ifndef NATIVE_TEST
	MAEvent *event = new MAEvent;
	event->type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_POINTER_PRESSED;
	eventData->widgetHandle = handle;
	event->data = eventData;
	Base::gEventQueue.put(*event);
#endif	
}

- (void)addChild: (IWidget*)child {
	[super addChild:child];
}

- (void)removeChild: (IWidget*)child {
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"text"]) {
		UIButton* button = (UIButton*) view;
		[button setTitle:value forState:UIControlStateNormal];
	} else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
