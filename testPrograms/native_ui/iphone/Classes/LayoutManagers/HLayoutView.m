#import "HLayoutView.h"

@implementation HLayoutView

- (CGSize)layoutSubviewsEffectively:(BOOL)effectively
{
    int total_width = leftMargin, max_height = 0;
    for (UIView *child in self.subviews)
    {
        total_width += child.frame.size.width;
        if (max_height < child.frame.size.height)
            max_height = child.frame.size.height;
    }
    total_width += ([self.subviews count] - 1) * spacing + rightMargin;
    if (effectively == YES)
    {
        int left, top, baseline = (self.frame.size.height - topMargin
                                   - bottomMargin) / 2 + topMargin;
        switch (hAlignment)
        {
            case UIControlContentHorizontalAlignmentLeft:
                left = leftMargin;
                break;
            case UIControlContentHorizontalAlignmentRight:
                left = self.frame.size.width - total_width + leftMargin;
                break;
            default: // center
                left = self.frame.size.width / 2 - total_width / 2 + leftMargin;
                break;
        }
        left -= spacing;
        for (UIView *child in self.subviews)
        {
            left += spacing;
            switch (vAlignment)
            {
                case UIControlContentVerticalAlignmentTop:
                    top = topMargin;
                    break;
                case UIControlContentVerticalAlignmentBottom:
                    top = self.frame.size.height - bottomMargin
                        - child.frame.size.height;
                    break;
                default: // center
                    top = baseline - child.frame.size.height / 2;
                    break;
            }
            child.frame = CGRectMake(left, top, child.frame.size.width,
                                     child.frame.size.height);
            left += child.frame.size.width;
        }
    }
    return CGSizeMake(total_width, topMargin + max_height + bottomMargin);
}

@end
