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

/**
 * @file SegmentedListViewItemWidget.h
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewItemWidget widget interface file.
 * A SegmentedListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in SegmentedListViewWidget objects.
 */

#import <Foundation/Foundation.h>

#import "IWidget.h"

/**
 * @brief A SegmentedListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in SegmentedListViewWidget objects.
 */
@interface SegmentedListViewItemWidget : IWidget
{
    /**
     * Cell that will be displayed inside SegmentedListViewWidget.
     */
    UITableViewCell* _cell;

    /**
     * Cell's height.
     */
    CGFloat _height;
}

@property(nonatomic, readonly) UITableViewCell* cell;
@property(nonatomic, assign) CGFloat height;

/**
 * Init function.
 */
- (id)init;

/**
 * Sets a property.
 * @param key The property of the widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key;

@end
