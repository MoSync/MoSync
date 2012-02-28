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
 * @file SegmentedListViewSection.mm
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewSection widget implementation file.
 * A SegmentedListViewSection object wraps one or more SegmentedListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */
#import "SegmentedListViewSection.h"
#import "SegmentedListViewItemWidget.h"

@implementation SegmentedListViewSection

@synthesize title = _title;
@synthesize headerTitle = _headerTitle;
@synthesize footerTitle = _footerTitle;

/**
 * Init function.
 */
-(id) init
{
    self = [super init];
    if (self)
    {
        _cells = [[NSMutableArray alloc] init];
    }

    return self;
}

/**
 * Return the number of contained cells.
 */
-(NSInteger) countCells
{
    return [_cells count];
}

/**
 * Add a given cell.
 * @param cell Cell to be added to section.
 */
-(void) addCell:(SegmentedListViewItemWidget*) cell
{
    [_cells addObject:cell];
}

/**
 * Remove a given cell from section.
 * @param cell Cell to be removed.
 */
-(void) removeCell:(SegmentedListViewItemWidget*) cell
{
    [_cells removeObject:cell];
}

/**
 * Get a cell at a given index.
 * @param index Cell's index.
 * @return Cell at the given index if the index is valid, nil otherwise.
 */
-(SegmentedListViewItemWidget*) cellWidgetAtIndex:(NSInteger) index
{
    if (index < 0 || index >= [_cells count])
    {
        return nil;
    }

    return [_cells objectAtIndex:index];
}

/**
 * Release all member objects.
 */
-(void) dealloc
{
    [_cells release];

    [super dealloc];
}
@end
