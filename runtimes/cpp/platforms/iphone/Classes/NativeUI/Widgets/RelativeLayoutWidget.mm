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

@interface RelativeLayoutWidget (hidden)

/**
 * Set the content offset.
 * @param value Coordinate left-top as following: "coordX-coordY".
 * @return MAW_RES_OK in case of success, or MAW_RES_INVALID_PROPERTY_VALUE if value
 * string does not have the desired format.
 */
-(int) setContentOffset:(NSString*) value;

/**
 * Get the content offset.
 * @return Content offset as following: "coordX-coordY".
 */
-(NSString*) getContentOffset;

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
	if([key isEqualToString:@MAW_RELATIVE_LAYOUT_SCROLLABLE]) {
		MoSyncTouchEnabledScrollView* sv = (MoSyncTouchEnabledScrollView*)view;
        BOOL enabled =  [value boolValue];
		sv.scrollEnabled = enabled;
	}
    else if ([key isEqualToString:@MAW_RELATIVE_LAYOUT_CONTENT_OFFSET])
    {
        return [self setContentOffset:value];
    }
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

- (NSString*)getPropertyWithKey: (NSString*)key
{
    if ([key isEqualToString:@MAW_RELATIVE_LAYOUT_CONTENT_OFFSET])
    {
        return [[self getContentOffset] retain];
    }
    else {
        return [super getPropertyWithKey:key];
    }
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


@implementation RelativeLayoutWidget (hidden)

/**
 * Set the content offset.
 * @param value Coordinate left-top as following: "coordX-coordY".
 * @return MAW_RES_OK in case of success, or MAW_RES_INVALID_PROPERTY_VALUE if value
 * string does not have the desired format.
 */
-(int) setContentOffset:(NSString*) value
{
    NSArray* components = [value componentsSeparatedByString:@"-"];
    if ([components count] != 2)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    CGFloat xCoord = [[components objectAtIndex:0] floatValue];
    CGFloat yCoord = [[components objectAtIndex:1] floatValue];
    CGPoint contentOffset = CGPointMake(xCoord / getScreenScale(), yCoord / getScreenScale());
    MoSyncTouchEnabledScrollView* scrollView = (MoSyncTouchEnabledScrollView*) view;
    [scrollView setContentOffset:contentOffset];
    return MAW_RES_OK;
}

/**
 * Get the content offset.
 * @return Content offset as following: "coordX-coordY".
 */
-(NSString*) getContentOffset
{
    MoSyncTouchEnabledScrollView* scrollView = (MoSyncTouchEnabledScrollView*) view;
    CGPoint contentOffset = scrollView.contentOffset;
    NSString* returnValue = [NSString stringWithFormat:@"%f-%f",
                             contentOffset.x * getScreenScale(),
                             contentOffset.y * getScreenScale()];
    return returnValue;
}

@end
