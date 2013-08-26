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

#import "ReflectionWidget.h"
#import <objc/runtime.h>
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation ReflectionWidget

- (id)initWithName:(NSString*)name
{
    self = [super init];
    if (self)
    {
        widgetClass = NSClassFromString(name);
        if(widgetClass == nil)
        {
            [self dealloc];
            return nil;
        }

        id object = [[widgetClass alloc] init];
        self.view = object;
        [object release];
        object = NULL;
    }
	return self;
}

- (void)addChild: (IWidget*)child
{
	[super addChild:child toSubview:YES];
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
	Ivar nameVar = class_getInstanceVariable(widgetClass, [key UTF8String]);
	if(nameVar != nil)
    {
		object_setIvar(widgetClass, nameVar, value);
	}
    else
    {
		return MAW_RES_ERROR;
	}

	return MAW_RES_OK;
}

@end
