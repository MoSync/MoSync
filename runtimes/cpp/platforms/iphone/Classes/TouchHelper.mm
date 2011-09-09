/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include "TouchHelper.h"

@implementation TouchHelper

- (id) init {
	touchArray = [[NSMutableArray alloc] init];
	return self;
}

- (void) dealloc {
    [touchArray release];
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
