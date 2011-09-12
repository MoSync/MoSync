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

#import "RelativeLayoutWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, UIScrollView)

@implementation RelativeLayoutWidget

- (id)init {	
    MoSyncUIScrollView* scrollView = [[MoSyncUIScrollView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
	view = scrollView;
    scrollView.scrollEnabled = NO;
	[scrollView setWidget:self];
	id ret = [super init];
    
    // if it doesn't scroll it shouldn't absorb events :)
    scrollView.userInteractionEnabled = NO;
    return ret;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"isScrollable"]) {
		MoSyncUIScrollView* sv = (MoSyncUIScrollView*)view;
        BOOL enabled =  [value boolValue];
		sv.scrollEnabled = enabled;
        sv.userInteractionEnabled = enabled;
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;	
}

- (void)layoutSubviews:(UIView*)_view {
	int minX = 0xffffff;
	int maxX =  -0xffffff;
	int minY = 0xffffff;
	int maxY =  -0xffffff;
	
	[super layoutSubviews:_view];
	
	for (IWidget *child in children)
    {
		UIView* childView = [child getView];
		
		if(childView.frame.origin.x < minX)
			minX = childView.frame.origin.x;
			
		if(childView.frame.origin.y < minY)
			minY = childView.frame.origin.y;
			
		if(childView.frame.origin.x + childView.frame.size.width > maxX)
			maxX = childView.frame.origin.x+childView.frame.size.width;

		if(childView.frame.origin.y + childView.frame.size.height > maxY)
			maxY = childView.frame.origin.y+childView.frame.size.height;
	}
	
	MoSyncUIScrollView* sv = (MoSyncUIScrollView*)view;
	//sv.contentSize = CGSizeMake(maxX-minX, maxY-minY);
	sv.contentSize = CGSizeMake(maxX, maxY); // negative values aren't supported.
}

@end
