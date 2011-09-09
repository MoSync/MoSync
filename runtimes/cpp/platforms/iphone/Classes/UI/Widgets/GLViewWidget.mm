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

#import "GLViewWidget.h"
#import "MoSyncGLView.h"
#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>
#include <base/Syscall.h>

@implementation GL2ViewWidget
-(id)init {
    return [super initWithApi:kEAGLRenderingAPIOpenGLES2];
}
@end

@implementation GLViewWidget

- (id)init {
    return [self initWithApi:kEAGLRenderingAPIOpenGLES1];
}

- (id)initWithApi:(EAGLRenderingAPI)api {
	view = [[[MoSyncGLView alloc] initWithFrame:CGRectMake(0, 0, 100, 100) 
                                         andApi:api] retain]; // TODO: do have to do this (retain)??
    if(view == nil) return nil;
	id ret = [super init];
	view.backgroundColor = [UIColor colorWithHexString:@"000000ff"];
    return ret;
}

- (void)setWidgetHandle:(int)_handle {
	[super setWidgetHandle:_handle];
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_GL_VIEW_READY;
	eventData->widgetHandle = _handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"bind"]) {
		MoSyncGLView* glView = (MoSyncGLView*) view;
		[glView bindContext];
	}
	else if([key isEqualToString:@"invalidate"]) {
		MoSyncGLView* glView = (MoSyncGLView*) view;		
		[glView renderContext];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;	
}

- (NSString*)getPropertyWithKey: (NSString*)key {
	
	return [super getPropertyWithKey:key];
}

- (void)show {
	[super show];
	//MoSyncGLView* glView = (MoSyncGLView*) view;
	//[glView viewAppeared];
}

@end
