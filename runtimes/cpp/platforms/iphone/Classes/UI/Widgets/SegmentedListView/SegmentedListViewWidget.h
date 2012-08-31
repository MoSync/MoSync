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
 * @file SegmentedListViewWidget.h
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListView widget interface file.
 * A SegmentedListView widget is used to present an indexed/grouped list of items.
 */

#import <Foundation/Foundation.h>

#import "IWidget.h"

// Forward declaration
@class SegmentedListViewSectionWidget;

/**
 * @brief A SegmentedListView widget is used to present an indexed/grouped list of items.
 * Implements UITableViewDelegate and UITableViewDelegate for filling table's content.
 */
@interface SegmentedListViewWidget : IWidget<UITableViewDelegate, UITableViewDataSource>
{
    /**
     * Native list/table view object.
     */
    UITableView* _tableView;

    /**
     * Sections for table view(SegmentedListViewSectionWidget objects).
     * Each section contains one or more cells, title, header and footer text.
     */
    NSMutableArray* _sections;
}

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

/**
 * Add a section to list.
 */
- (void) addChild: (SegmentedListViewSectionWidget*) section;

@end
