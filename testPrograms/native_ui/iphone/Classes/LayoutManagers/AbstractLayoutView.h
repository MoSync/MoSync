@interface AbstractLayoutView : UIScrollView
{
    int spacing;
    int leftMargin, rightMargin;
    int topMargin, bottomMargin;
    UIControlContentHorizontalAlignment hAlignment;
    UIControlContentVerticalAlignment vAlignment;
}

- (id)init;
- (id)initWithFrame:(CGRect)frame;
- (id)initWithFrame:(CGRect)frame spacing:(int)s;
- (id)initWithFrame:(CGRect)frame spacing:(int)s leftMargin:(int)lm
    rightMargin:(int)rm topMargin:(int)tm bottomMargin:(int)bm;
// UIControlContentHorizontalAlignmentFill and
// UIControlContentVerticalAlignmentFill are not supported, since layout
// managers do not manage subviews sizes.
- (id)initWithFrame:(CGRect)frame spacing:(int)s leftMargin:(int)lm
    rightMargin:(int)rm topMargin:(int)tm bottomMargin:(int)bm
    hAlignment:(UIControlContentHorizontalAlignment)ha
    vAlignment:(UIControlContentVerticalAlignment)va;

// These functions set the layout view's size to sizeThatFits: (without resizing
// the subviews). The caller can override one of the dimensions.
- (void)setSize;
- (void)setSizeWithWidth:(int)w;
- (void)setSizeWithHeight:(int)h;

// Returns frame width / height minus margins.
- (int)availableWidth;
- (int)availableHeight;

- (void)scrollToShow:(UIView *)subview animated:(BOOL)animated;

- (CGSize)layoutSubviewsEffectively:(BOOL)effectively;

@end
