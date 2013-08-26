/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

            // Force child to have smaller/equal width with its parrent in the context of vertical layout.
            int maxViewWidth = self.width - totalHorizontalMargin;
            if ( viewWidth > maxViewWidth )
            {
                viewWidth = maxViewWidth;
            }
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
    AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
    float maxWidth = 0.0;
    float countHeight = ([alv getTopMargin] + [alv getBottomMargin]);
    for (IWidget* child in _children)
    {
        countHeight += child.height;
        maxWidth = MAX(maxWidth, child.width);
    }
    return CGSizeMake(maxWidth, countHeight);
}

@end