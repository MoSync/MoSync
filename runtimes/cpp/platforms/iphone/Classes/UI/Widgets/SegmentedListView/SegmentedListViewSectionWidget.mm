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
 * @file SegmentedListViewSectionWidget.mm
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewSectionWidget widget implementation file.
 * A SegmentedListViewSectionWidget object wraps one or more SegmentedListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */
#import "SegmentedListViewSectionWidget.h"
#import "SegmentedListViewItemWidget.h"

@implementation SegmentedListViewSectionWidget

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
 * Sets a property.
 * @param key The property of the widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    int resultCode = MAW_RES_OK;

    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_TITLE])
    {
        self.title = value;
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_HEADER])
    {
        self.headerTitle = value;
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_FOOTER])
    {
        self.footerTitle = value;
    }
    else
    {
        resultCode = [super setPropertyWithKey:key toValue:value];
    }

    return resultCode;
}

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    NSString* returnValue;
    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_TITLE])
    {
        returnValue = [self.title retain];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_HEADER])
    {
        returnValue = [self.headerTitle retain];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_SECTION_FOOTER])
    {
        returnValue = [self.footerTitle retain];
    }
    else
    {
        returnValue = [super getPropertyWithKey:key];
    }

    return returnValue;
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
-(void) addChild:(SegmentedListViewItemWidget*) cell
{
    LOGIN;
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
