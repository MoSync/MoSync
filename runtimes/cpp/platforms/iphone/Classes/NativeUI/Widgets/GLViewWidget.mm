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
#import "UIColorExpanded.h"
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

- (id)initWithApi:(EAGLRenderingAPI)api
{
    self = [super init];
    if (self)
    {
        self.view = [[[MoSyncGLView alloc] initWithFrame:CGRectMake(0, 0, 100, 100) andApi:api] autorelease];
        if(self.view == nil)
        {
            return nil;
        }
        self.view.backgroundColor = [UIColor colorWithHexString:@"000000ff"];
    }
    return self;
}

- (void)setHandle:(int)_handle {
	[super setHandle:_handle];
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_GL_VIEW_READY;
	eventData->widgetHandle = _handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@MAW_GL_VIEW_BIND]) {
		MoSyncGLView* glView = (MoSyncGLView*) self.view;
		[glView bindContext];
	}
	else if([key isEqualToString:@MAW_GL_VIEW_INVALIDATE]) {
		MoSyncGLView* glView = (MoSyncGLView*) self.view;
		[glView renderContext];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

@end
