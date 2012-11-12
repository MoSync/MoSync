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

#import "HorizontalLayoutWidget.h"
#import "LayoutWidgets.h"
#import "LayoutManagers.h"

@implementation HorizontalLayoutWidget

/**
 * Init function.
 */
- (id)init
{
	return [super init:OrientationHorizontal];
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
    float countWidth = 0;
    for (IWidget *child in _children)
    {
        float viewHeight = child.height;
        float viewWidth = child.width;

        if(child.autoSizeHeight == WidgetAutoSizeFillParent &&
           self.parent)
        {
            viewHeight = self.height - totalVerticalMargin;
        }
        else if(child.autoSizeHeight == WidgetAutoSizeWrapContent &&
            self.parent)
        {
            viewHeight = [child sizeThatFitsForWidget].height;
        }

        switch (child.autoSizeWidth)
        {
            case WidgetAutoSizeFillParent:
                [fillParentWidgets addObject:child];
                break;
            case WidgetAutoSizeWrapContent:
                viewWidth = [child sizeThatFitsForWidget].width;
                countWidth += viewWidth;
                break;
            case WidgetAutoSizeFixed:
                countWidth += viewWidth;
                break;
        }
        child.size = CGSizeMake(viewWidth, viewHeight);
    }

    if ([fillParentWidgets count] != 0)
    {
        float fillParentWidth = self.width - countWidth - totalHorizontalMargin;
        fillParentWidth -=  [alv getSpacing] * ([_children count] - 1);
        fillParentWidth /= [fillParentWidgets count];
        for (IWidget *child in fillParentWidgets)
        {
            child.size = CGSizeMake(fillParentWidth, child.height);
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
    float countWidth = 0.0;
    float maxHeight = 0.0;
    for (IWidget* child in _children)
    {
        countWidth += child.width;
        maxHeight = MAX(maxHeight, child.height);
    }
    return CGSizeMake(countWidth, maxHeight);
}

@end