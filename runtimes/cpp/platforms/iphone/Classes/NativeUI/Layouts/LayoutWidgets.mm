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
#import "WidgetLayoutingMacro.h"

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
        self.autoSizeWidth = WidgetAutoSizeFixed;
        self.autoSizeHeight = WidgetAutoSizeFixed;
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
    INNativeUILog;
    [super addChild:child toSubview:YES];
    return MAW_RES_OK;
}

/**
 * Layout its subviews.
 * Called from view's layoutSubview method.
 * @param view UIView object that triggered the layout event.
 */
-(void)layoutSubviews:(UIView*)view
{
    INNativeUILog;
	if (_orientation == OrientationVertical)
    {
        [self verticalLayout];
	}
    else if (_orientation == OrientationHorizontal)
    {
        [self horizontalLayout];
	}
}

/**
 * Layout subviews for horizontal layout widget.
 */
-(void)horizontalLayout
{
    INNativeUILog;
    AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
	float totalHorizontalMargin = ([alv getLeftMargin] + [alv getRightMargin]);
	float totalVerticalMargin = ([alv getTopMargin] + [alv getBottomMargin]);
    NSMutableArray* fillParentWidgets = [[NSMutableArray alloc] initWithCapacity:[_children count]];
    float countWidth = 0;
    for (IWidget *child in _children)
    {
        float viewHeight = child.height;
        float viewWidth = child.width;

        if(child.autoSizeHeight == WidgetAutoSizeFillParent)
        {
            viewHeight = self.height - totalVerticalMargin;
        }
        else if(child.autoSizeHeight == WidgetAutoSizeWrapContent)
        {
            viewHeight = [child sizeThatFitsForWidget].height;
        }

        if (child.autoSizeWidth == WidgetAutoSizeFillParent)
        {
            [fillParentWidgets addObject:child];
        }
        else if (child.autoSizeWidth == WidgetAutoSizeWrapContent)
        {
            viewWidth = [child sizeThatFitsForWidget].width;
            countWidth += viewWidth;
        }
        else if (child.autoSizeWidth == WidgetAutoSizeFixed)
        {
            countWidth += viewWidth;
        }

        child.size = CGSizeMake(viewWidth, viewHeight);
    }

    if ([fillParentWidgets count] != 0)
    {
        float fillParentWidth = (self.width - countWidth - totalHorizontalMargin - [alv getSpacing] * ([_children count] - 1))/[fillParentWidgets count];
        NSLog(@"float = %f", fillParentWidth);
        for (IWidget *child in fillParentWidgets)
        {
            child.size = CGSizeMake(fillParentWidth, child.height);
        }
    }
    [fillParentWidgets release];
    fillParentWidgets = nil;
    [((MoSyncHLayoutView*) self.view) superLayoutSubviews];
    OUTNativeUILog;
}

/**
 * Layout subviews for vertical layout widget.
 */
-(void)verticalLayout
{
    INNativeUILog;
    AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
	float totalHorizontalMargin = ([alv getLeftMargin] + [alv getRightMargin]);
	float totalVerticalMargin = ([alv getTopMargin] + [alv getBottomMargin]);

    NSMutableArray* fillParentWidgets = [[NSMutableArray alloc] initWithCapacity:[_children count]];
    float countHeight = 0;
    for (IWidget *child in _children)
    {
        float viewWidth = child.width;
        float viewHeight = child.height;

        if(child.autoSizeWidth == WidgetAutoSizeFillParent)
        {
            viewWidth = self.width - totalHorizontalMargin;
        }
        else if(child.autoSizeWidth == WidgetAutoSizeWrapContent)
        {
            viewWidth = [child sizeThatFitsForWidget].width;
        }

        if(child.autoSizeHeight == WidgetAutoSizeFillParent)
        {
            [fillParentWidgets addObject:child];
        }
        else if(child.autoSizeHeight == WidgetAutoSizeWrapContent)
        {
            viewHeight = [child sizeThatFitsForWidget].height;
            countHeight += viewHeight;
        }
        else if(child.autoSizeHeight == WidgetAutoSizeFixed)
        {
            countHeight += viewHeight;
        }

        child.size = CGSizeMake(viewWidth, viewHeight);
    }

    if ([fillParentWidgets count] != 0)
    {
        float fillParentHeight = (self.height - countHeight - totalVerticalMargin -
                                [alv getSpacing] *  ([_children count] - 1)) / [fillParentWidgets count];
        NSLog(@"fillParentHeight = %f", fillParentHeight);
        for (IWidget *child in _children)
        {
            child.size = CGSizeMake(child.width, fillParentHeight);
        }
    }
    [fillParentWidgets release];
    fillParentWidgets = nil;
    [((MoSyncVLayoutView*)self.view) superLayoutSubviews];
    OUTNativeUILog;
}

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	// maybe set scroll indicator style..

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

@end



@implementation HorizontalLayoutWidget
- (id)init {
	return [super init:OrientationHorizontal];
}

/**
 * Asks the layout to calculate and return the size that best fits its subviews.
 * @return The size that best fits its subviews.
 */
- (CGSize)sizeThatFitsForWidget
{
    INNativeUILog;
    float countWidth = 0.0;
    float maxHeight = self.height;
    for (IWidget* child in _children)
    {
        countWidth += child.width;
        maxHeight = MAX(maxHeight, child.height);
    }
    OUTNativeUILog;
    return CGSizeMake(countWidth, maxHeight);
}

@end

@implementation VerticalLayoutWidget
- (id)init {
	return [super init:OrientationVertical];
}

/**
 * Asks the layout to calculate and return the size that best fits its subviews.
 * @return The size that best fits its subviews.
 */
- (CGSize)sizeThatFitsForWidget
{
    INNativeUILog;
    float maxWidth = self.width;
    float countHeight = 0.0;
    for (IWidget* child in _children)
    {
        countHeight += child.height;
        maxWidth = MAX(maxWidth, child.width);
    }
    OUTNativeUILog;
    return CGSizeMake(maxWidth, countHeight);
}

@end