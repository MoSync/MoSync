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
#import "AbstractLayoutView.h"
#import "WidgetLayoutingMacro.h"

@interface AbstractLayoutView (AbstractLayoutViewExpanded)
- (void) setVerticalAlignment: (UIControlContentVerticalAlignment) va;
- (void) setHorizontalAlignment: (UIControlContentHorizontalAlignment) ha;
- (void) setSpacing:(int)_spacing;
- (void) setLeftMargin:(int)_margin;
- (void) setRightMargin:(int)_margin;
- (void) setTopMargin:(int)_margin;
- (void) setBottomMargin:(int)_margin;

- (int)getLeftMargin;
- (int)getRightMargin;
- (int)getTopMargin;
- (int)getBottomMargin;
- (int)getSpacing;
@end


typedef enum {
	OrientationVertical,
	OrientationHorizontal
} LinearLayoutOrientation;

@interface LinearLayoutBase : IWidget
{
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
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return MAW_RES_OK if the child was added, MAW_RES_INVALID_INDEX if the index param
 * was invalid.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index;

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - #MAW_RES_ERROR otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Recalculate its and children size.
 * If needed and possible the parent will be resized too.
 */
- (void)layout;

- (void)superLayoutSubviews;


@end
