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

#import "VLayoutView.h"

@implementation VLayoutView

- (CGSize)layoutSubviewsEffectively:(BOOL)effectively
{
    int total_height = topMargin, max_width = 0;
    for (UIView *child in self.subviews)
    {
        total_height += child.frame.size.height;
        if (max_width < child.frame.size.width)
            max_width = child.frame.size.width;
    }
    total_height += ([self.subviews count] - 1) * spacing + bottomMargin;
    if (effectively == YES)
    {
        int left, top, baseline = (self.frame.size.width - leftMargin
                                   - rightMargin) / 2 + leftMargin;
        switch (vAlignment)
        {
            case UIControlContentVerticalAlignmentTop:
                top = topMargin;
                break;
            case UIControlContentVerticalAlignmentBottom:
                top = self.frame.size.height - total_height + topMargin;
                break;
            default: // center
                top = self.frame.size.height / 2 - total_height / 2 + topMargin;
                break;
        }
        top -= spacing;
        for (UIView *child in self.subviews)
        {
            top += spacing;
            switch (hAlignment)
            {
                case UIControlContentHorizontalAlignmentLeft:
                    left = leftMargin;
                    break;
                case UIControlContentHorizontalAlignmentRight:
                    left = self.frame.size.width - rightMargin
                        - child.frame.size.width;
                    break;
                default: // center
                    left = baseline - child.frame.size.width / 2;
                    break;
            }

			CGFloat childFrameWidth =  child.frame.size.width;
			CGFloat childFrameHeight = child.frame.size.height;
            child.frame = CGRectMake(left, top, childFrameWidth, childFrameHeight);
            top += child.frame.size.height;
        }
    }
    return CGSizeMake(leftMargin + max_width + rightMargin, total_height);
}

@end
