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

#import "LayoutWidgets.h"
#import "LayoutManagers.h"
#import "WidgetUtils.h"

@implementation AbstractLayoutView (AbstractLayoutViewExpanded)

- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va {
	vAlignment = va;
	[self setNeedsLayout];
}

- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha {
	hAlignment = ha;
	[self setNeedsLayout];
}

- (void) setSpacing:(int)_spacing {
	spacing = _spacing;
	[self setNeedsLayout];
}

- (void) setLeftMargin:(int)_margin {
	leftMargin = _margin;
	[self setNeedsLayout];
}

- (void) setRightMargin:(int)_margin {
	rightMargin = _margin;
	[self setNeedsLayout];
}

- (void) setTopMargin:(int)_margin {
	topMargin = _margin;
	[self setNeedsLayout];
}

- (void) setBottomMargin:(int)_margin {
	bottomMargin = _margin;
	[self setNeedsLayout];
}

- (int) getLeftMargin {
	return leftMargin;
}

- (int) getRightMargin {
	return rightMargin;
}

- (int) getTopMargin{
	return topMargin;;
}

- (int) getBottomMargin {
	return bottomMargin;
}

- (int) getSpacing {
	return spacing;
}
@end

MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, VLayoutView)
MAKE_UIWRAPPER_LAYOUTING_IMPLEMENTATION(MoSync, HLayoutView)

@implementation LinearLayoutBase

/**
 * Init function.
 * @param orientation Layout's orientation.
 */
- (id)init:(LinearLayoutOrientation)orientation
{
	_orientation = orientation;
    self = [super init];
    if (self)
    {
        AbstractLayoutView* layout = nil;
        if(_orientation == OrientationVertical)
        {
            MoSyncVLayoutView* vLayout = [[MoSyncVLayoutView alloc]  initWithFrame:CGRectZero spacing:0
                                        leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
                                        hAlignment:UIControlContentHorizontalAlignmentLeft
                                        vAlignment:UIControlContentVerticalAlignmentTop];
            [vLayout setWidget:self];
            layout = vLayout;
        }
        else if (_orientation == OrientationHorizontal)
        {
            MoSyncHLayoutView* hLayout = [[MoSyncHLayoutView alloc]  initWithFrame:CGRectZero spacing:0
                                        leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
                                        hAlignment:UIControlContentHorizontalAlignmentLeft
                                        vAlignment:UIControlContentVerticalAlignmentTop];
            [hLayout setWidget:self];
            layout = hLayout;
        }

        self.view = layout;
        self.autoSizeWidth = WidgetAutoSizeWrapContent;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        [layout release];
        layout = NULL;
    }
	return self;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return MAW_RES_OK.
 */
- (int)addChild:(IWidget*)child
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
 * - #MAW_RES_ERROR otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
	if([key isEqualToString:@MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		if([value isEqualToString:@"top"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentTop];
		else if([value isEqualToString:@"center"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentCenter];
		else if([value isEqualToString:@"bottom"])
			[alv setVerticalAlignment:UIControlContentVerticalAlignmentBottom];
	} else if([key isEqualToString:@MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		if([value isEqualToString:@"left"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
		else if([value isEqualToString:@"center"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentCenter];
		else if([value isEqualToString:@"right"])
			[alv setHorizontalAlignment:UIControlContentHorizontalAlignmentRight];
	} else if([key isEqualToString:@MAW_HORIZONTAL_LAYOUT_PADDING_LEFT]) {
        // Is applied to MAW_VERTICAL_LAYOUT_PADDING_LEFT too.
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		[alv setLeftMargin:[value intValue]];
	} else if([key isEqualToString:@MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT]) {
        // Is applied to MAW_VERTICAL_LAYOUT_PADDING_RIGHT too.
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		[alv setRightMargin:[value intValue]];
	} else if([key isEqualToString:@MAW_HORIZONTAL_LAYOUT_PADDING_TOP]) {
        // Is applied to MAW_VERTICAL_LAYOUT_PADDING_TOP too.
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		[alv setTopMargin:[value intValue]];
	} else if([key isEqualToString:@MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM]) {
        // Is applied to MAW_VERTICAL_LAYOUT_PADDING_BOTTOM too.
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		[alv setBottomMargin:[value intValue]];
	} else if([key isEqualToString:@"spacing"]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		[alv setSpacing:[value intValue]];
	}
	else if([key isEqualToString:@MAW_VERTICAL_LAYOUT_SCROLLABLE]) {
		AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
		alv.scrollEnabled = [value boolValue];
	}
	else {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;
}

/**
 * Recalculate its and children size.
 * If needed and possible the parent will be resized too.
 */
- (void)layout
{
    CGSize sizeThatFits = [self sizeThatFitsForWidget];
    float width = self.width;
    float height = self.height;
    if (self.autoSizeWidth == WidgetAutoSizeWrapContent)
    {
        width = sizeThatFits.width;
    }
    else if (self.autoSizeWidth == WidgetAutoSizeFillParent &&
             width < sizeThatFits.width)
    {
        width = sizeThatFits.width;
    }
    if (self.autoSizeHeight == WidgetAutoSizeWrapContent)
    {
        height = sizeThatFits.height;
    }
    else if (self.autoSizeHeight == WidgetAutoSizeFillParent &&
             height < sizeThatFits.height)
    {
        height = sizeThatFits.height;
    }

    self.size = CGSizeMake(width, height);
    [self layoutSubviews:self.view];

    // If the widget has parent and if at least one of its auto size params is wrap content,
    // then ask the parent to layout itself.
    if (self.parent &&
        (self.parent.autoSizeWidth != WidgetAutoSizeFixed ||
         self.parent.autoSizeHeight != WidgetAutoSizeFixed))
    {
        [self.parent layout];
    }
}

- (void)superLayoutSubviews
{
    if (_orientation == OrientationVertical)
    {
        [((MoSyncHLayoutView*)self.view) superLayoutSubviews];
    }
    else if (_orientation == OrientationHorizontal)
    {
        [((MoSyncVLayoutView*)self.view) superLayoutSubviews];
    }
}

@end
