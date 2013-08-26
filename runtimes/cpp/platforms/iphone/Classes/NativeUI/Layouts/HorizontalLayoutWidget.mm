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
			if (viewHeight > self.height)
			{
				viewHeight = self.height;
			}
        }

        switch (child.autoSizeWidth)
        {
            case WidgetAutoSizeFillParent:
                [fillParentWidgets addObject:child];
                break;
            case WidgetAutoSizeWrapContent:
                viewWidth = [child sizeThatFitsForWidget].width;
                countWidth += viewWidth;

                // If width of the children excedes the available width force the child to fill the available space.
                if ( countWidth > (self.width - totalHorizontalMargin) )
                {
                    countWidth -= viewWidth;
                    [fillParentWidgets addObject:child];
                }
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
    AbstractLayoutView* alv = (AbstractLayoutView*)self.view;
    float countWidth = ([alv getLeftMargin] + [alv getRightMargin]); //Add horizontal padding.
    float maxHeight = 0.0;

    for (IWidget* child in _children)
    {
        countWidth += child.width;
        maxHeight = MAX(maxHeight, child.height);
    }
    maxHeight += ([alv getTopMargin] + [alv getBottomMargin]); //Add vertical padding.
    return CGSizeMake(countWidth, maxHeight);
}

@end