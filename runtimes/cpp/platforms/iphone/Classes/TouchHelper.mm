#include "TouchHelper.h"

@implementation TouchHelper

- (id) init {
	touchArray = [[NSMutableArray alloc] init];
	return self;
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
