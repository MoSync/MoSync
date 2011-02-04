#import "AbstractLayoutView.h"

@implementation AbstractLayoutView

- (id)init
{
    return [self initWithFrame:CGRectZero spacing:0
            leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
            hAlignment:UIControlContentHorizontalAlignmentCenter
            vAlignment:UIControlContentVerticalAlignmentCenter];
}

- (id)initWithFrame:(CGRect)frame
{
    return [self initWithFrame:frame spacing:0
            leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
            hAlignment:UIControlContentHorizontalAlignmentCenter
            vAlignment:UIControlContentVerticalAlignmentCenter];
}

- (id)initWithFrame:(CGRect)frame spacing:(int)s
{
    return [self initWithFrame:frame spacing:s
            leftMargin:0 rightMargin:0 topMargin:0 bottomMargin:0
            hAlignment:UIControlContentHorizontalAlignmentCenter
            vAlignment:UIControlContentVerticalAlignmentCenter];
}

- (id)initWithFrame:(CGRect)frame spacing:(int)s leftMargin:(int)lm
        rightMargin:(int)rm topMargin:(int)tm bottomMargin:(int)bm
{
    return [self initWithFrame:frame spacing:s
            leftMargin:lm rightMargin:rm topMargin:tm bottomMargin:bm
            hAlignment:UIControlContentHorizontalAlignmentCenter
            vAlignment:UIControlContentVerticalAlignmentCenter];    
}

- (id)initWithFrame:(CGRect)frame spacing:(int)s leftMargin:(int)lm
    rightMargin:(int)rm topMargin:(int)tm bottomMargin:(int)bm
    hAlignment:(UIControlContentHorizontalAlignment)ha
    vAlignment:(UIControlContentVerticalAlignment)va
{
    if (self = [super initWithFrame:frame])
    {
        spacing = s;
        leftMargin = lm;
        rightMargin = rm;
        topMargin = tm;
        bottomMargin = bm;
        hAlignment = ha;
        vAlignment = va;
        
        self.scrollEnabled = NO;
    }
    return self;
}

- (void)setSize
{
    BOOL autoresizesSubviews = self.autoresizesSubviews;
    self.autoresizesSubviews = NO;
    [self sizeToFit];
    self.autoresizesSubviews = autoresizesSubviews;
}

- (void)setSizeWithWidth:(int)w
{
    BOOL autoresizesSubviews = self.autoresizesSubviews;
    self.autoresizesSubviews = NO;
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, w,
                            [self sizeThatFits:self.frame.size].height);
    self.autoresizesSubviews = autoresizesSubviews;
}

- (void)setSizeWithHeight:(int)h
{
    BOOL autoresizesSubviews = self.autoresizesSubviews;
    self.autoresizesSubviews = NO;
    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y,
                            [self sizeThatFits:self.frame.size].width, h);
    self.autoresizesSubviews = autoresizesSubviews;
}

- (int)availableWidth
{
    return self.frame.size.width - leftMargin - rightMargin;
}

- (int)availableHeight
{
    return self.frame.size.height - topMargin - bottomMargin;
}

- (void)scrollToShow:(UIView *)subview animated:(BOOL)animated
{
    CGRect frame = [subview.superview convertRect:subview.frame toView:self];
    CGRect rect = CGRectMake(frame.origin.x + self.contentInset.left,
                             frame.origin.y + self.contentInset.top,
                             frame.size.width,
                             frame.size.height);
    [self scrollRectToVisible:rect animated:animated];
}

- (CGSize)layoutSubviewsEffectively:(BOOL)effectively
{
    return CGSizeZero;
}

- (void)layoutSubviews
{
    BOOL showsHorizontalScrollIndicator = self.showsHorizontalScrollIndicator;
    BOOL showsVerticalScrollIndicator = self.showsVerticalScrollIndicator;
    self.showsHorizontalScrollIndicator = NO;
    self.showsVerticalScrollIndicator = NO;
    CGSize contentSize = [self layoutSubviewsEffectively:YES];
    self.showsHorizontalScrollIndicator = showsHorizontalScrollIndicator;
    self.showsVerticalScrollIndicator = showsVerticalScrollIndicator;
    CGFloat overflowLeft = 0, overflowTop = 0;
    if (self.frame.size.width < contentSize.width
        && hAlignment != UIControlContentHorizontalAlignmentLeft)
    {
        if (hAlignment == UIControlContentHorizontalAlignmentRight)
            overflowLeft = contentSize.width - self.frame.size.width;
        else // center
            overflowLeft = (contentSize.width - self.frame.size.width) / 2;
    }
    if (self.frame.size.height < contentSize.height
        && vAlignment != UIControlContentVerticalAlignmentTop)
    {
        if (vAlignment == UIControlContentVerticalAlignmentBottom)
            overflowTop = contentSize.height - self.frame.size.height;
        else // center
            overflowTop = (contentSize.height - self.frame.size.height) / 2;
    }
    self.contentSize = contentSize;
    self.contentInset = UIEdgeInsetsMake(overflowTop, overflowLeft,
                                         -overflowTop, -overflowLeft);
}

- (CGSize)sizeThatFits:(CGSize)size
{
    BOOL showsHorizontalScrollIndicator = self.showsHorizontalScrollIndicator;
    BOOL showsVerticalScrollIndicator = self.showsVerticalScrollIndicator;
    self.showsHorizontalScrollIndicator = NO;
    self.showsVerticalScrollIndicator = NO;
    CGSize contentSize = [self layoutSubviewsEffectively:NO];
    self.showsHorizontalScrollIndicator = showsHorizontalScrollIndicator;
    self.showsVerticalScrollIndicator = showsVerticalScrollIndicator;    
    return contentSize;
}

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
