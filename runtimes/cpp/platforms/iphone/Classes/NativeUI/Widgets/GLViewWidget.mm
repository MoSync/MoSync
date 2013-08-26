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
