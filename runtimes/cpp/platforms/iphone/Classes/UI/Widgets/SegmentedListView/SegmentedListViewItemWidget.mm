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
#import "NSStringExpanded.h"

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

/**
 * Set the selected state of the item.
 * @param value If "true" the item will be selected, if "false" the item
 * item will be unselected.
 * @param animated If "true" will animate the transition between selected states,
 * if "false" will do the transition immediate.
 * @return MAW_RES_OK if the params have valid values, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setSelected:(NSString*) value
          animated:(NSString*) animated;

/**
 * Set the highlighted state of the item.
 * @param value If "true" the item will be highlighted, if "false" the item
 * item will be unhighlighted.
 * @param animated If "true" will animate the transition between highlighted states,
 * if "false" will do the transition immediate.
 * @return MAW_RES_OK if the params have valid values, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setHighlighted:(NSString*) value
             animated:(NSString*) animated;

/**
 * Set the type of standard accessory used by the item(normal state).
 * @param accessoryTypeString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @param state If YES, the accessory will be set for normal mode, otherwise it will be set for
 * editing mode.
 * @return MAW_RES_OK if the accessoryTypeString is valid, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setAccessoryType:(NSString*) accessoryTypeString
             normalState:(BOOL) state;

/**
 * Get the string constant assocciated with an accessory type.
 * @param accessoryType A cell accessory type.
 * @return One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 */
-(NSString*) getAccessoryType:(UITableViewCellAccessoryType) accessoryType;

/**
 * Get the cell accessory type assocciated with a string constant.
 * @param accessoryTypeString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @return A cell accessory type. If the accessoryTypeString is invalid UITableViewCellAccessoryNone
 * is returned.
 */
-(UITableViewCellAccessoryType) getAccessoryTypeFromString:(NSString*) accessoryTypeString;

/**
 * Check if a given string contains one of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE values.
 * @return YES if it contains, NO otherwise.
 */
-(BOOL) isAccessoryTypeValid:(NSString*) accessoryTypeString;

/**
 * Set the editing style.
 * @param editingStyleString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE constants.
 * @return MAW_RES_OK if the editingStyleString is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setEditingStyleProperty:(NSString*) editingStyleString;

/**
 * Get the editing style.
 * @return One of the MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE constants.
 */
-(NSString*) getEditingStyleProperty;

/**
 * Set the reorder control flag.
 * @param value "true" or "false" string.
 * @return MAW_RES_OK if the value param is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setShowReorderControlProperty:(NSString*) value;

@end

@implementation SegmentedListViewItemWidget

@synthesize cell = _cell;
@synthesize deleteButtonTitle = _deleteButtonTitle;
@synthesize height = _height;
@synthesize editingStyle = _editingStyle;
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

    [self layoutSubviews:view];
	[child layout];
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
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE])
    {
        self.deleteButtonTitle = value;
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SET_SELECTED])
    {
        return [self setSelected:@"true" animated:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SET_UNSELECTED])
    {
        return [self setSelected:@"false" animated:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SET_HIGHLIGHTED])
    {
        return [self setHighlighted:@"true" animated:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SET_UNHIGHLIGHTED])
    {
        return [self setHighlighted:@"false" animated:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE])
    {
        return [self setAccessoryType:value normalState:YES];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT])
    {
        return [self setAccessoryType:value normalState:NO];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SHOW_REORDER_CONTROL])
    {
        return [self setShowReorderControlProperty:value];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
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
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE])
    {
        return [[NSString alloc] initWithString:self.deleteButtonTitle];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_IS_SHOWING_DELETE_CONFIRMATION])
    {
        const char* value = _cell.showingDeleteConfirmation ? "true" : "false";
        return [[NSString alloc] initWithFormat:@"%s", value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_IS_SELECTED])
    {
        const char* value = _cell.isSelected ? "true" : "false";
        return [[NSString alloc] initWithFormat:@"%s", value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_IS_HIGHLIGHTED])
    {
        const char* value = _cell.isHighlighted ? "true" : "false";
        return [[NSString alloc] initWithFormat:@"%s", value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE])
    {
        NSString* type = [self getAccessoryType:_cell.accessoryType];
        return [type retain];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ITEM_SHOW_REORDER_CONTROL])
    {
        const char* value = _cell.showsReorderControl ? "true" : "false";
        return [[NSString alloc] initWithFormat:@"%s", value];
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
    [_deleteButtonTitle release];
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

/**
 * Set the selected state of the item.
 * @param value If "true" the item will be selected, if "false" the item
 * item will be unselected.
 * @param animated If "true" will animate the transition between selected states,
 * if "false" will do the transition immediate.
 * @return MAW_RES_OK if the params have valid values, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setSelected:(NSString*) value
          animated:(NSString*) animated
{
    if (![value isEqualToString:@"false"]    &&
        ![value isEqualToString:@"true"]     &&
        ![animated isEqualToString:@"false"] &&
        ![animated isEqualToString:@"true"])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [_cell setSelected:[value boolValue] animated:[animated boolValue]];
    return MAW_RES_OK;
}

/**
 * Set the highlighted state of the item.
 * @param value If "true" the item will be highlighted, if "false" the item
 * item will be unhighlighted.
 * @param animated If "true" will animate the transition between highlighted states,
 * if "false" will do the transition immediate.
 * @return MAW_RES_OK if the params have valid values, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setHighlighted:(NSString*) value
             animated:(NSString*) animated
{
    if (![value isEqualToString:@"false"]    &&
        ![value isEqualToString:@"true"]     &&
        ![animated isEqualToString:@"false"] &&
        ![animated isEqualToString:@"true"])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [_cell setHighlighted:[value boolValue] animated:[animated boolValue]];
    return MAW_RES_OK;
}

/**
 * Set the type of standard accessory used by the item(normal state).
 * @param accessoryTypeString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @param state If YES, the accessory will be set for normal mode, otherwise it will be set for
 * editing mode.
 * @return MAW_RES_OK if the accessoryTypeString is valid, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setAccessoryType:(NSString*) accessoryTypeString
             normalState:(BOOL) state
{
    if (![self isAccessoryTypeValid:accessoryTypeString])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    UITableViewCellAccessoryType type = [self getAccessoryTypeFromString:accessoryTypeString];
    if (state)
    {
        _cell.accessoryType = type;
    }
    else
    {
        _cell.editingAccessoryType = type;
    }
    return MAW_RES_OK;
}

/**
 * Get the string constant assocciated with an accessory type.
 * @param accessoryType A cell accessory type.
 * @return One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 */
-(NSString*) getAccessoryType:(UITableViewCellAccessoryType) accessoryType
{
    int accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
    switch (accessoryType)
    {
        case UITableViewCellAccessoryDisclosureIndicator:
            accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE;
            break;
        case UITableViewCellAccessoryDetailDisclosureButton:
            accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL;
            break;
        case UITableViewCellAccessoryCheckmark:
            accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK;
            break;
        case UITableViewCellAccessoryNone:
        default:
            accessoryTypeInt = MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
            break;
    }

    return [NSString stringWithFormat:@"%d", accessoryTypeInt];
}

/**
 * Get the cell accessory type assocciated with a string constant.
 * @param accessoryTypeString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @return A cell accessory type. If the accessoryTypeString is invalid UITableViewCellAccessoryNone
 * is returned.
 */
-(UITableViewCellAccessoryType) getAccessoryTypeFromString:(NSString*) accessoryTypeString
{
    UITableViewCellAccessoryType accessoryType =  UITableViewCellAccessoryNone;
    int accessoryTypeInt = [accessoryTypeString intValue];
    switch (accessoryTypeInt)
    {
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
            accessoryType = UITableViewCellAccessoryDisclosureIndicator;
            break;
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
            accessoryType = UITableViewCellAccessoryDetailDisclosureButton;
            break;
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
            accessoryType = UITableViewCellAccessoryCheckmark;
            break;
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
        default:
            accessoryType = UITableViewCellAccessoryNone;
            break;
    }
    return accessoryType;
}

/**
 * Check if a given string contains one of the MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE values.
 * @return YES if it contains, NO otherwise.
 */
-(BOOL) isAccessoryTypeValid:(NSString*) accessoryTypeString
{
    if (![accessoryTypeString canParseNumber])
    {
        return NO;
    }
    BOOL returnValue = NO;
    switch ([accessoryTypeString intValue])
    {
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
        case MAW_SEGMENTED_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
            returnValue = YES;
            break;
        default:
            returnValue = NO;
            break;
    }
    return returnValue;
}

/**
 * Set the editing style.
 * @param editingStyleString One of the MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE constants.
 * @return MAW_RES_OK if the editingStyleString is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setEditingStyleProperty:(NSString*) editingStyleString
{
    if (![editingStyleString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UITableViewCellEditingStyle style = UITableViewCellEditingStyleNone;
    int returnValue = MAW_RES_OK;
    switch ([editingStyleString intValue])
    {
        case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_DELETE:
            style = UITableViewCellEditingStyleDelete;
            break;
        case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_INSERT:
            style = UITableViewCellEditingStyleInsert;
            break;
        case MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_NONE:
            style = UITableViewCellEditingStyleNone;
            break;
        default:
            returnValue = MAW_RES_INVALID_PROPERTY_VALUE;
            break;
    }
    if (returnValue != MAW_RES_OK)
    {
        self.editingStyle = style;
    }
    return returnValue;
}

/**
 * Get the editing style.
 * @return One of the MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE constants.
 */
-(NSString*) getEditingStyleProperty
{
    int style;
    switch (_cell.editingStyle)
    {
        case UITableViewCellEditingStyleDelete:
            style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_DELETE;
            break;
        case UITableViewCellEditingStyleInsert:
            style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_INSERT;
            break;
        case UITableViewCellEditingStyleNone:
        default:
            style = MAW_SEGMENTED_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
            break;
    }
    return [NSString stringWithFormat:@"%d", style];
}

/**
 * Set the reorder control flag.
 * @param value "true" or "false" string.
 * @return MAW_RES_OK if the value param is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setShowReorderControlProperty:(NSString*) value
{
    if (![value isEqualToString:@"false"] &&
        ![value isEqualToString:@"true"])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    _cell.showsReorderControl = YES;
    return MAW_RES_OK;
}

@end