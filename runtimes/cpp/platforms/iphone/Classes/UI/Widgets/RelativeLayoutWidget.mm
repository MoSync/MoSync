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

@interface MoSyncTouchEnabledScrollView : MoSyncUIScrollView {
@private
}
@end
@implementation MoSyncTouchEnabledScrollView
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.scrollEnabled == NO)
        [[self nextResponder] touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.scrollEnabled == NO)
        [[self nextResponder] touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.scrollEnabled == NO)
        [[self nextResponder] touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (self.scrollEnabled == NO)
        [[self nextResponder] touchesCancelled:touches withEvent:event];
}
@end

@implementation RelativeLayoutWidget

- (id)init {
    MoSyncTouchEnabledScrollView* scrollView = [[MoSyncTouchEnabledScrollView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
	view = scrollView;
    scrollView.scrollEnabled = NO;
    scrollView.userInteractionEnabled = YES;
	[scrollView setWidget:self];
	id ret = [super init];

    return ret;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	if([key isEqualToString:@"isScrollable"]) {
		MoSyncTouchEnabledScrollView* sv = (MoSyncTouchEnabledScrollView*)view;
        BOOL enabled =  [value boolValue];
		sv.scrollEnabled = enabled;
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

		if(childView.frame.origin.x + childView.bounds.size.width > maxX)
			maxX = childView.frame.origin.x+childView.bounds.size.width;

		if(childView.frame.origin.y + childView.bounds.size.height > maxY)
			maxY = childView.frame.origin.y+childView.bounds.size.height;
	}

	MoSyncTouchEnabledScrollView* sv = (MoSyncTouchEnabledScrollView*)view;
	//sv.contentSize = CGSizeMake(maxX-minX, maxY-minY);
	sv.contentSize = CGSizeMake(maxX, maxY); // negative values aren't supported.
}

@end
