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
 * @file SegmentedListViewItemWidget.mm
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListViewItemWidget widget implementation file.
 * A SegmentedListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in SegmentedListViewWidget objects.
 */


#import "SegmentedListViewItemWidget.h"

/**
 * Private methods for SegmentedListViewItemWidget.
 */
@interface SegmentedListViewItemWidget(hidden)

/**
 * Set cell editable property value.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setEditableProperty:(NSString*) value;

/**
 * Check if cell is editable.
 * @return "true" is cell is editable, "false" otherwise.
 */
-(NSString*) isEditableProperty;

/**
 * Enable / disable moving the cell.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setCanMoveProperty:(NSString*) value;

/**
 * Check if cell can be moved.
 * @return "true" if cell can be moved, "false" otherwise.
 */
-(NSString*) canMoveProperty;

@end

@implementation SegmentedListViewItemWidget

@synthesize cell = _cell;
@synthesize height = _height;
@synthesize editable = _canEdit;
@synthesize canMove = _canMove;

static NSString* kReuseIdentifier = @"Cell";

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:kReuseIdentifier];
        _height = _cell.bounds.size.height;
        view = [_cell.contentView retain];

        _canEdit = YES;
        _canMove = YES;
        autoSizeParamX = FILL_PARENT;
        autoSizeParamY = FILL_PARENT;
    }

    return self;
}

/**
 * Add a child to list item.
 */
- (void)addChild: (IWidget*) child
{
    LOGIN;
    UIView* childView = [child getView];
	[child setParent:self];
	[children addObject:child];

    [_cell.contentView addSubview:childView];

    NSLog(@"_cell.contentView = %f", _cell.contentView.bounds.size.width);

    [self layoutSubviews:view];
//	[child layout];
}

/**
 * Sets a property.
 * @param key The property of the widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT])
    {
        return [self setEditableProperty:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_MOVE])
    {
        return [self setCanMoveProperty:value];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }
}

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT])
    {
        return [[self isEditableProperty] retain];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_MOVE])
    {
        return [[self canMoveProperty] retain];
    }
    else
    {
        return [super getPropertyWithKey:key];
    }
}

/**
 * Release all member data.
 */
-(void) dealloc
{
    // Do NOT release _cell object here. It will be released by the super class.

    [super dealloc];
}

@end

@implementation SegmentedListViewItemWidget(hidden)

/**
 * Set cell editable property value.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setEditableProperty:(NSString*) value
{
    if (![value isEqualToString:@"false"] &&
        ![value isEqualToString:@"true"])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.editable = [value boolValue];
    return MAW_RES_OK;
}

/**
 * Check if cell is editable.
 * @return "true" if cell is editable, "false" otherwise.
 */
-(NSString*) isEditableProperty
{
    NSString* returnValue = self.editable ? @"true" : @"false";
    return returnValue;
}

/**
 * Enable / disable moving the cell.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setCanMoveProperty:(NSString*) value
{
    if (![value isEqualToString:@"false"] &&
        ![value isEqualToString:@"true"])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.canMove = [value boolValue];
    return MAW_RES_OK;
}

/**
 * Check if cell can be moved.
 * @return "true" if cell can be moved, "false" otherwise.
 */
-(NSString*) canMoveProperty
{
    NSString* returnValue = self.canMove ? @"true" : @"false";
    return returnValue;
}

@end