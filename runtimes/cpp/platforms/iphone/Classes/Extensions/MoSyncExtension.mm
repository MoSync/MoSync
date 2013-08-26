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

#include <objc/runtime.h>
#include "MoSyncExtension.h"
#include <helpers/cpp_defs.h>

static NSMutableArray* sExtensions;

void initExtensions(MoSync* mosync)
{
	sExtensions = [[NSMutableArray alloc] init ];
	int numClasses = objc_getClassList(NULL, 0);
	Class *classes = NULL;
	classes = (Class*)malloc(sizeof(Class) * numClasses);
	numClasses = objc_getClassList(classes, numClasses);

	for(int i = 0; i < numClasses; i++)
	{
		if(classes[i] && class_conformsToProtocol(classes[i], @protocol(MoSyncExtensionProtocol)))
		{
			id extension = [[classes[i] alloc] initWithMoSync:mosync];
			[sExtensions addObject:extension];
		}
	}

	free((void*) classes);
}

#if 0
@interface TestExtension : NSObject<MoSyncExtensionProtocol>
{
}

-(long long) maScreenGrabberGetScreen:(int)image;

@end

@implementation TestExtension
-(id) initWithMoSync:(MoSync*)mosync
{
	return self;
}

-(void) close
{
}

-(NSString*) getName
{
	return @"ScreenGrabber";
}

-(int) getHash
{
	return 0x1234124;
}

-(long long) invokeExtensionWithIndex:(int)_index andArg1:(int)a andArg2:(int)b andArg3:(int)c
{
	switch(_index)
    {
		case 1:
			return [self maScreenGrabberGetScreen:a];
	}
	return MA_EXTENSION_FUNCTION_UNAVAILABLE;
}

-(long long) maScreenGrabberGetScreen:(int)image
{
	return 0;
}
@end

#endif // if 0
