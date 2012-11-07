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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "RelativeLayoutWidget.h"
#import "WidgetLayoutingMacro.h"

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, UIScrollView)

@interface MoSyncTouchEnabledScrollView : MoSyncUIScrollView
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

/**
 * Recalculate scroll view's content size.
 */
-(void)resizeContent;

@end

@implementation RelativeLayoutWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        MoSyncTouchEnabledScrollView* scrollView = [[MoSyncTouchEnabledScrollView alloc] initWithFrame:CGRectMake(0, 0, 100, 60)];
        self.view = scrollView;
        scrollView.scrollEnabled = NO;
        scrollView.userInteractionEnabled = YES;

        [scrollView setWidget:self];
        [scrollView release];
        scrollView = nil;

        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        self.autoSizeWidth = WidgetAutoSizeWrapContent;
    }
    return self;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return MAW_RES_OK.
 */
-(int)addChild:(IWidget*)child
{
    [super addChild:child toSubview:YES];
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return MAW_RES_OK if the child was added, MAW_RES_INVALID_INDEX if the index param
 * was invalid.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
    return [super insertChild:child atIndex:index toSubview:YES];
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_RELATIVE_LAYOUT_SCROLLABLE])
    {
		MoSyncTouchEnabledScrollView* sv = (MoSyncTouchEnabledScrollView*) self.view;
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

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    if ([key isEqualToString:@MAW_RELATIVE_LAYOUT_CONTENT_OFFSET])
    {
        return [[self getContentOffset] retain];
    }
    else {
        return [super getPropertyWithKey:key];
    }
}

/**
 * Layout its subviews.
 * Called from view's layoutSubview method.
 * @param view UIView object that triggered the layout event.
 */
- (void)layoutSubviews:(UIView*)view
{
    for (IWidget *child in _children)
	{
		int viewWidth = child.width;
		int viewHeight = child.height;

		if (child.autoSizeWidth == WidgetAutoSizeFillParent)
        {
			viewWidth = self.width;
		}
		else if (child.autoSizeWidth == WidgetAutoSizeWrapContent)
        {
			viewWidth = [child sizeThatFitsForWidget].width;
		}

		if (child.autoSizeHeight == WidgetAutoSizeFillParent)
        {
			viewHeight = self.height;
		}
		else if (child.autoSizeHeight == WidgetAutoSizeWrapContent)
        {
			viewHeight = [child sizeThatFitsForWidget].height;
		}

		child.size = CGSizeMake(viewWidth, viewHeight);
	}

    [self resizeContent];
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
    UIScrollView* scrollView = (UIScrollView*) self.view;
    [scrollView setContentOffset:contentOffset];
    return MAW_RES_OK;
}

/**
 * Get the content offset.
 * @return Content offset as following: "coordX-coordY".
 */
-(NSString*) getContentOffset
{
    UIScrollView* scrollView = (UIScrollView*) self.view;
    CGPoint contentOffset = scrollView.contentOffset;
    NSString* returnValue = [NSString stringWithFormat:@"%f-%f",
                             contentOffset.x * getScreenScale(),
                             contentOffset.y * getScreenScale()];
    return returnValue;
}

/**
 * Recalculate scroll view's content size.
 */
-(void)resizeContent
{
	int minX = 0xffffff;
	int maxX =  -0xffffff;
	int minY = 0xffffff;
	int maxY =  -0xffffff;

	for (IWidget *child in _children)
    {
		if(child.originX < minX)
			minX = child.originX;

		if(child.originY < minY)
			minY = child.originY;

		if(child.originX + child.width > maxX)
			maxX = child.originX + child.width;

		if(child.originY + child.height > maxY)
			maxY = child.originY + child.height;
	}

	MoSyncTouchEnabledScrollView* scrollView = (MoSyncTouchEnabledScrollView*)self.view;
	scrollView.contentSize = CGSizeMake(maxX, maxY);
}

@end
