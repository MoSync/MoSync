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

#import <Foundation/Foundation.h>
#import "IWidget.h"

@interface AbstractLayoutView (AbstractLayoutViewExpanded)
- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va;
- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha;
- (void) setSpacing:(int)_spacing;
- (void) setLeftMargin:(int)_margin;
- (void) setRightMargin:(int)_margin;
- (void) setTopMargin:(int)_margin;
- (void) setBottomMargin:(int)_margin;
@end


typedef enum {
	OrientationVertical,
	OrientationHorizontal
} LinearLayoutOrientation;

@interface LinearLayoutBase : IWidget {
    /**
     * Layout's orientation.
     */
	LinearLayoutOrientation _orientation;

}

/**
 * Init function.
 * @param orientation Layout's orientation.
 */
-(id)init:(LinearLayoutOrientation)orientation;

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return MAW_RES_OK.
 */
-(int)addChild:(IWidget*)child;

/**
 * Layout its subviews.
 * Called from view's layoutSubview method.
 * @param view UIView object that triggered the layout event.
 */
-(void)layoutSubviews:(UIView*)view;

/**
 * Layout subviews for horizontal layout widget.
 */
-(void)horizontalLayout;

/**
 * Layout subviews for vertical layout widget.
 */
-(void)verticalLayout;

- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

@end

@interface HorizontalLayoutWidget : LinearLayoutBase {
}
- (id)init;

/**
 * Asks the layout to calculate and return the size that best fits its subviews.
 * @return The size that best fits its subviews.
 */
- (CGSize)sizeThatFitsForWidget;

@end

@interface VerticalLayoutWidget : LinearLayoutBase {
}

- (id)init;

/**
 * Asks the layout to calculate and return the size that best fits its subviews.
 * @return The size that best fits its subviews.
 */
- (CGSize)sizeThatFitsForWidget;

@end
