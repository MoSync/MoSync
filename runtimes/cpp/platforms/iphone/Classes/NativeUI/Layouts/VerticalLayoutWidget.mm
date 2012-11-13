/*
 Copyright (C) 2012 MoSync AB

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
#import "VerticalLayoutWidget.h"

@implementation VerticalLayoutWidget

/**
 * Init function.
 */
- (id)init
{
	return [super init:OrientationVertical];
}

/**
 * Layout its subviews.
 * Called from view's layoutSubview method.
 * @param view UIView object that triggered the layout event.
 */
- (void)layoutSubviews:(UIView*)view
{
    AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
	float totalHorizontalMargin = ([alv getLeftMargin] + [alv getRightMargin]);
	float totalVerticalMargin = ([alv getTopMargin] + [alv getBottomMargin]);

    NSMutableArray* fillParentWidgets = [[NSMutableArray alloc] initWithCapacity:[_children count]];
    float countHeight = 0;
    for (IWidget *child in _children)
    {
        float viewWidth = child.width;
        float viewHeight = child.height;

        if (child.autoSizeWidth == WidgetAutoSizeFillParent &&
            self.parent)
        {
            viewWidth = self.width - totalHorizontalMargin;
        }
        else if(child.autoSizeWidth == WidgetAutoSizeWrapContent &&
            self.parent)
        {
            viewWidth = [child sizeThatFitsForWidget].width;
        }

        switch (child.autoSizeHeight)
        {
            case WidgetAutoSizeFillParent:
                [fillParentWidgets addObject:child];
                break;
            case WidgetAutoSizeWrapContent:
                viewHeight = [child sizeThatFitsForWidget].height;
                countHeight += viewHeight;
                break;
            case WidgetAutoSizeFixed:
                countHeight += viewHeight;
                break;
        }

        child.size = CGSizeMake(viewWidth, viewHeight);
    }

    if ([fillParentWidgets count] != 0)
    {
        float fillParentHeight = self.height - countHeight - totalVerticalMargin;
        fillParentHeight -= [alv getSpacing] *  ([_children count] - 1);
        fillParentHeight /= [fillParentWidgets count];
        for (IWidget *child in fillParentWidgets)
        {
            child.size = CGSizeMake(child.width, fillParentHeight);
        }
    }
    [fillParentWidgets release];
    fillParentWidgets = nil;
    [super superLayoutSubviews];
}

/**
 * Asks the layout to calculate and return the size that best fits its subviews.
 * @return The size that best fits its subviews.
 */
- (CGSize)sizeThatFitsForWidget
{
    float maxWidth = 0.0;
    float countHeight = 0.0;
    for (IWidget* child in _children)
    {
        countHeight += child.height;
        maxWidth = MAX(maxWidth, child.width);
    }
    return CGSizeMake(maxWidth, countHeight);
}

@end