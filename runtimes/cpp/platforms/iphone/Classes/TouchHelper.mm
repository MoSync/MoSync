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

#include "TouchHelper.h"

@implementation TouchHelper

- (id) init {
	touchArray = [[NSMutableArray alloc] init];
	return self;
}

- (void) dealloc {
    [touchArray release];
    [super dealloc];
}

- (int) addTouch: (UITouch*) touch 
{	
	int index = 0;
	for (UITouch* cur in touchArray) {
		if(touch == cur) {
			return -1;
		} else if(cur == (UITouch*)[NSNull null]) {
			[touchArray replaceObjectAtIndex:index withObject:touch];
			return index;
		}
		
		index++;
	}
	
	[touchArray addObject:touch];
	return index;
	
}

- (int) getTouchId: (UITouch*) touch 
{
	return [touchArray indexOfObject:touch];
}

- (void) removeTouch: (UITouch*) touch 
{
	int index = 0;
	for (UITouch* cur in touchArray) {
		if(cur == touch) {
			[touchArray replaceObjectAtIndex: index withObject:[NSNull null]];
			return;
		}
		index++;
	}	
}

- (void) clearTouches {
	[touchArray removeAllObjects];
}

@end
