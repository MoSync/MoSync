//
//  LabelWidget.mm
//  nativeuitest
//
//  Created by Niklas Nummelin on 11/26/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "GLViewWidget.h"
#import "MoSyncGLView.h"

#ifndef NATIVE_TEST
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>
#endif

@implementation GLViewWidget

- (id)init {
	view = [[[MoSyncGLView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)] retain]; // TODO: do have to do this (retain)??
	return [super init];	
}

- (void)wasCreated {
#ifndef NATIVE_TEST
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = WIDGET_EVENT_GL_VIEW_READY;
	eventData->widgetHandle = handle;
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
	if([key isEqualToString:@"bind"]) {
		[view bindContext];
	}
	else if([key isEqualToString:@"invalidate"]) {
		[view renderContext];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MA_WIDGET_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

@end
