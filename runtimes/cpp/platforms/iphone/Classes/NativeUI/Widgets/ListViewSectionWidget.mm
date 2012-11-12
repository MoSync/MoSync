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
 * @file ListViewSectionWidget.mm
 * @author Bogdan Iusco
 * @date 07 Aug 2012
 *
 * @brief ListViewSectionWidget widget implementaion file.
 * A ListViewSectionWidget object wraps one or more ListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */

#import "ListViewSectionWidget.h"
#import "ListViewItemWidget.h"

@implementation ListViewSectionWidget

@synthesize delegate = _delegate;
@synthesize title = _title;
@synthesize headerTitle = _headerTitle;
@synthesize footerTitle = _footerTitle;
@synthesize index = _index;

/**
 * Init function.
 */
-(id) init
{
    self = [super init];
    return self;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not ListViewItemWidget.
 */
- (int)addChild:(IWidget*)child
{
    if ([child class] != [ListViewItemWidget class] &&
        [child superclass] != [ListViewItemWidget class])
    {
        return MAW_RES_INVALID_LAYOUT;
    }
    [super addChild:child toSubview:NO];
    ListViewItemWidget* item = (ListViewItemWidget*)child;
    item.delegate = self;
    int row = [_children count] - 1;
    NSIndexPath* indexPath = [NSIndexPath indexPathForRow:row inSection:_index];
    [_delegate insertItemAtIndexPath:indexPath];
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not ListViewItemWidget.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
    if ([child class] != [ListViewItemWidget class] &&
        [child superclass] != [ListViewItemWidget class])
    {
        return MAW_RES_INVALID_LAYOUT;
    }
    int result = [super insertChild:child atIndex:index toSubview:NO];
    if (result != MAW_RES_OK)
    {
        return result;
    }
    NSIndexPath* indexPath = [NSIndexPath indexPathForRow:[index intValue] inSection:_index];
    [_delegate insertItemAtIndexPath:indexPath];
    ListViewItemWidget* item = (ListViewItemWidget*)child;
    item.delegate = self;
    return MAW_RES_OK;
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child
{
    int row = [_children indexOfObject:child];
    NSIndexPath*  indexPath = [NSIndexPath indexPathForRow:row inSection:_index];
	[self removeChild:child fromSuperview:NO];
    [_delegate deleteItemAtIndexPath:indexPath];
}

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    int resultCode = MAW_RES_OK;

    if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_TITLE])
    {
        self.title = value;
        [_delegate reloadListViewSectionIndexTitles];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_HEADER])
    {
        self.headerTitle = value;
        [_delegate reloadListViewSectionAtIndex:_index];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_FOOTER])
    {
        self.footerTitle = value;
        [_delegate reloadListViewSectionAtIndex:_index];
    }
    else
    {
        resultCode = [super setPropertyWithKey:key toValue:value];
    }

    return resultCode;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    NSString* returnValue;
    if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_TITLE])
    {
        returnValue = [self.title retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_HEADER])
    {
        returnValue = [self.headerTitle retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_SECTION_FOOTER])
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
    return [_children count];
}

/**
 * Remove a given cell from section.
 * @param cell Cell to be removed.
 */
-(void) removeCell:(ListViewItemWidget*) cell
{
    [_children removeObject:cell];
}

/**
 * Get a cell at a given index.
 * @param index Cell's index.
 * @return Cell at the given index if the index is valid, nil otherwise.
 */
-(ListViewItemWidget*) cellWidgetAtIndex:(NSInteger) index
{
    if (index < 0 || index >= [_children count])
    {
        return nil;
    }

    return [_children objectAtIndex:index];
}

#pragma mark ListViewSectionWidgetDelegate

/**
 * Notify that an list item's size has changed.
 * @param listItem List item that changed its size.
 */
-(void)sizeChangedFor:(ListViewItemWidget*) listItem
{
    int row = [_children indexOfObject:listItem];
    NSIndexPath* indexPath = [NSIndexPath indexPathForRow:row inSection:_index];
    [_delegate reloadItemAtIndexPath:indexPath];
}

/**
 * Ask the list section for the item's height.
 * @return The item height set in the table view.
 */
- (CGFloat)tableViewSectionRowHeight
{
    return [self.delegate tableViewRowHeight];
}

/**
 * Release all member objects.
 */
-(void) dealloc
{
    [_headerTitle release];
    [_footerTitle release];
    [_title release];

    [super dealloc];
}

@end
