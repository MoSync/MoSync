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
 * @file SegmentedListViewSection.h
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewSection widget interface file.
 * A SegmentedListViewSection object wraps one or more SegmentedListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */

#import <Foundation/Foundation.h>

// Forward declaration.
@class SegmentedListViewItemWidget;

/**
 * @brief A SegmentedListViewSection object wraps one or more SegmentedListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */
@interface SegmentedListViewSection : NSObject
{
    /**
     * Section's title displayed on the right side of the table view.
     * The table view must be in the plain style.
     */
    NSString* _title;

    /**
     * A string used as the title of the section header.
     */
    NSString* _headerTitle;

    /**
     * A string used as the title of the section footer.
     */
    NSString* _footerTitle;

    /**
     * Cells for section.
     */
    NSMutableArray* _cells;
}

@property(nonatomic, retain) NSString* title;
@property(nonatomic, retain) NSString* headerTitle;
@property(nonatomic, retain) NSString* footerTitle;

/**
 * Return the number of contained cells.
 */
-(NSInteger) countCells;

/**
 * Add a given cell.
 * @param cell Cell to be added to section.
 */
-(void) addCell:(SegmentedListViewItemWidget*) cell;

/**
 * Remove a given cell from section.
 * @param cell Cell to be removed.
 */
-(void) removeCell:(SegmentedListViewItemWidget*) cell;

/**
 * Get a cell at a given index.
 * @param index Cell's index.
 * @return Cell at the given index if the index is valid, nil otherwise.
 */
-(SegmentedListViewItemWidget*) cellWidgetAtIndex:(NSInteger) index;

@end
