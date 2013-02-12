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

/**
 * @file ListViewItemWidget.mm
 * @author Bogdan Iusco
 *
 * @brief ListViewItemWidget widget implementation file.
 * A ListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in ListViewWidget objects.
 */

#import "ListViewItemWidget.h"
#import "UIColorExpanded.h"
#import "NSStringExpanded.h"
#import "HorizontalLayoutWidget.h"
#import "VerticalLayoutWidget.h"
#import "RelativeLayoutWidget.h"
#import "WidgetLayoutingMacro.h"

/**
 * Hidden functions/methods for ListViewItemWidget class
 */
@interface ListViewItemWidget (hidden)

/**
 * Set an icon of the list view item that is placed to the left of the text.
 * Setter for MAW_LIST_VIEW_ITEM_ICON.
 * @param iconHandle Handle to an uncompressed image resource.
 * @return One of the following constants:
 * - MAW_RES_OK if the icon was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the iconHandle param was invalid.
 */
-(int) setIconProperty:(NSString*) iconHandle;

/**
 * Set the type of list item displayed. Provides the corresponding icon in the right side of the list view.
 * Setter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE.
 * @param accessoryType One of "none", "hasChildren", "hasDetails" or "isChecked".
 * @return One of the following constants:
 * - MAW_RES_OK if the accessory type was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the accessoryType param was invalid.
 */
-(int) setAccessoryTypeProperty:(NSString*) accessoryType;

/**
 * Set the font color of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_COLOR.
 * @param fontColor  A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return One of the following constants:
 * - MAW_RES_OK if the font color was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font color param was invalid.
 */
-(int) setFontColorProperty:(NSString*) fontColor;

/**
 * Set the font size of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_SIZE.
 * @param fontSize  Font size, in points.
 * @return One of the following constants:
 * - MAW_RES_OK if the font size was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font size param was invalid.
 */
-(int) setFontSizeProperty:(NSString*) fontSize;

/**
 * Set the font handle of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_HANDLE.
 * @param fontHandle A font handle received from loading fonts using
 * maFontGetName and maFontLoadWithName syscalls.
 * @return One of the following constants:
 * - MAW_RES_OK if the font handle was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font handle param was invalid.
 */
-(int) setFontHandleProperty:(NSString*) fontHandle;

/**
 * Set the widget's background color.
 * Setter for MAW_WIDGET_BACKGROUND_COLOR.
 * @param backgroundColor  A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return One of the following constants:
 * - MAW_RES_OK if the background color was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the background color param was invalid.
 */
-(int) setBackgroundColorProperty:(NSString*) backgroundColor;

/**
 * Set cell editable property value.
 * Setter for MAW_LIST_VIEW_ITEM_EDIT.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setEditableProperty:(NSString*) value;

/**
 * Check if cell is editable.
 * Setter for MAW_LIST_VIEW_ITEM_EDIT.
 * @return "true" is cell is editable, "false" otherwise.
 */
-(NSString*) isEditableProperty;

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
 * Setter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT.
 * @param accessoryTypeString One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @param state If YES, the accessory will be set for normal mode, otherwise it will be set for
 * editing mode.
 * @return MAW_RES_OK if the accessoryTypeString is valid, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setAccessoryTypeIntProperty:(NSString*) accessoryTypeString
                       normalState:(BOOL) state;

/**
 * Get the string constant assocciated with an accessory type.
 * Getter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT.
 * @param accessoryType A cell accessory type.
 * @return One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 */
-(NSString*) getAccessoryTypeIntProperty:(UITableViewCellAccessoryType) accessoryType;

/**
 * Get the cell accessory type assocciated with a string constant.
 * @param accessoryTypeString One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @return A cell accessory type. If the accessoryTypeString is invalid UITableViewCellAccessoryNone
 * is returned.
 */
-(UITableViewCellAccessoryType) getAccessoryTypeIntFromString:(NSString*) accessoryTypeString;

/**
 * Check if a given string contains one of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE values.
 * @return YES if it contains, NO otherwise.
 */
-(BOOL) isAccessoryTypeIntValid:(NSString*) accessoryTypeString;

/**
 * Set the editing style.
 * Setter for MAW_LIST_VIEW_ITEM_EDITING_STYLE.
 * @param editingStyleString One of the MAW_LIST_VIEW_ITEM_EDIT_STYLE constants.
 * @return MAW_RES_OK if the editingStyleString is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setEditingStyleProperty:(NSString*) editingStyleString;

/**
 * Get the editing style.
 * Getter for MAW_LIST_VIEW_ITEM_EDITING_STYLE.
 * @return One of the MAW_LIST_VIEW_ITEM_EDIT_STYLE constants.
 */
-(NSString*) getEditingStyleProperty;

/**
 * Set the selection style.
 * Setter for MAW_LIST_VIEW_ITEM_SELECTION_STYLE.
 * @param selectionStyleString One of the MAW_LIST_VIEW_ITEM_SELECTION_STYLE constants.
 * @return MAW_RES_OK if the selectionStyleString is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setSelectionStyleProperty:(NSString*) selectionStyleString;

/**
 * Get the selection style.
 * Getter for MAW_LIST_VIEW_ITEM_SELECTION_STYLE.
 * @return One of the MAW_LIST_VIEW_ITEM_SELECTION_STYLE constants.
 */
-(NSString*) getSelectionStyleProperty;

@end

@implementation ListViewItemWidget

static NSString* kReuseIdentifier = @"Cell";

@synthesize delegate = _delegate;
@synthesize deleteButtonTitle = _deleteButtonTitle;
@synthesize editingStyle = _editingStyle;
@synthesize editable = _canEdit;

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UITableViewCell* moSyncCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:kReuseIdentifier];
        self.view = moSyncCell;
        [moSyncCell release];
        moSyncCell = nil;
        self.autoSizeHeight = WidgetAutoSizeWrapContent;
        self.autoSizeWidth = WidgetAutoSizeFillParent;
    }

    return self;
}

/**
 * Get item's cell.
 */
-(UITableViewCell*)cell
{
    UITableViewCell* cell = (UITableViewCell*) self.view;
    return cell;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * It is recommended that the child should be an layout.
 * @return MAW_RES_OK.
 */
- (int)addChild:(IWidget*)child
{
    if (_children.count ==0 )
    {
        [super addChild:child toSubview:YES];
    }
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * It is recommended that the child should be an layout.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
    return [super insertChild:child atIndex:index toSubview:YES];
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child
{
    [super removeChild:child fromSuperview:YES];
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
    if([key isEqualToString:@MAW_LIST_VIEW_ITEM_TEXT])
    {
        self.cell.textLabel.text = value;
		[self layout];
	}
	else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_ICON])
    {
        return [self setIconProperty:value];
	}
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE])
    {
        return [self setAccessoryTypeProperty:value];
	}
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_COLOR])
    {
        return [self setFontColorProperty:value];
    }
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_SIZE])
    {
        return [self setFontSizeProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_HANDLE])
    {
        [self setFontHandleProperty:value];
    }
	else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
        return [self setBackgroundColorProperty:value];
	}
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_EDIT])
    {
        return [self setEditableProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE])
    {
        self.deleteButtonTitle = value;
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SET_SELECTED])
    {
        return [self setSelected:kWidgetTrueValue animated:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SET_UNSELECTED])
    {
        return [self setSelected:kWidgetFalseValue animated:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SET_HIGHLIGHTED])
    {
        return [self setHighlighted:kWidgetTrueValue animated:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SET_UNHIGHLIGHTED])
    {
        return [self setHighlighted:kWidgetFalseValue animated:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT])
    {
        return [self setAccessoryTypeIntProperty:value normalState:YES];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT])
    {
        return [self setAccessoryTypeIntProperty:value normalState:NO];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_EDITING_STYLE])
    {
        return [self setEditingStyleProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SELECTION_STYLE])
    {
        return [self setSelectionStyleProperty:value];
    }
	else
    {
		return [super setPropertyWithKey:key toValue:value];
	}
	return MAW_RES_OK;

}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    if([key isEqualToString:@MAW_LIST_VIEW_ITEM_TEXT])
    {
        return [self.cell.textLabel.text retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_EDIT])
    {
        return [[self isEditableProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_DELETE_BUTTON_TITLE])
    {
        return [[self deleteButtonTitle] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_IS_SHOWING_DELETE_CONFIRMATION])
    {
        NSString* value = self.cell.showingDeleteConfirmation ? kWidgetTrueValue : kWidgetFalseValue;
        return [value retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_IS_SELECTED])
    {
        NSString* value = self.cell.isSelected ? kWidgetTrueValue : kWidgetFalseValue;
        return [value retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_IS_HIGHLIGHTED])
    {
        NSString* value = self.cell.isHighlighted ? kWidgetTrueValue : kWidgetFalseValue;
        return [value retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT])
    {
        NSString* type = [self getAccessoryTypeIntProperty:self.cell.accessoryType];
        return [type retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_EDIT])
    {
        NSString* type = [self getAccessoryTypeIntProperty:self.cell.editingAccessoryType];
        return [type retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_EDITING_STYLE])
    {
        return [[self getEditingStyleProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_SELECTION_STYLE])
    {
        return [[self getSelectionStyleProperty] retain];
    }
    else
    {
        return [super getPropertyWithKey:key];
    }
}

/**
 * Asks the widget to calculate and return the size that best fits its subviews.
 * @return The size that best fits itself.
 */
-(CGSize)sizeThatFitsForWidget
{
    CGSize size = self.size;
    if ([_children count] > 0)
    {
        IWidget* child = [_children objectAtIndex:0];
        size = child.size;
    }
    else if (self.autoSizeHeight != WidgetAutoSizeFixed)
    {
        // Fix for MOSYNC-2560.
        // The system resizes the cell's height after loading it, so if
        // the table loads the same cell for many times its height increases.
        // Setting the item's height with the rowHeight value from the table view will
        // prevent that from happening.
        size.height = [_delegate tableViewSectionRowHeight];
    }
    return  size;
}

/**
 * Recalculate its and children size.
 */
- (void)layout
{
    if ([_children count] == 0)
    {
        return;
    }

    CGSize previousSize = self.size;
    CGSize newSize;
    if (self.autoSizeHeight == WidgetAutoSizeFixed)
    {
        newSize = self.size;
    }
    else
    {
        newSize = [self sizeThatFitsForWidget];
    }

    if (!CGSizeEqualToSize(previousSize, newSize))
    {
        self.size = newSize;
        [_delegate sizeChangedFor:self];
    }
}

/**
 * Get widget's height.
 */
-(CGFloat) height
{
    return [self sizeThatFitsForWidget].height;
}

/**
 * Setter for MAW_WIDGET_HEIGHT.
 * @param value MAW_CONSTANT_FILL_AVAILABLE_SPACE, MAW_CONSTANT_WRAP_CONTENT or
 * an int value greater or equal to zero.
 * @return MAW_RES_OK if the height was set, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setHeightProperty:(NSString*) value
{
    int returnValue = [super setHeightProperty:value];
    if (returnValue == MAW_RES_OK &&
        [_children count] == 0)
    {
        // Its height has changed. Notify the table view.
        [_delegate sizeChangedFor:self];
    }
    return returnValue;
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    [_deleteButtonTitle release];

    [super dealloc];
}

@end


@implementation ListViewItemWidget (hidden)

/**
 * Set an icon of the list view item that is placed to the left of the text.
 * Setter for MAW_LIST_VIEW_ITEM_ICON.
 * @param iconHandle Handle to an uncompressed image resource.
 * @return One of the following constants:
 * - MAW_RES_OK if the icon was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the iconHandle param was invalid.
 */
-(int) setIconProperty:(NSString*) iconHandle
{
    if (![iconHandle canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    int handle = [iconHandle intValue];
    if (handle <= 0)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(handle);
    self.cell.imageView.image = [UIImage imageWithCGImage:imageResource->image];
    return MAW_RES_OK;
}

/**
 * Set the type of list item displayed. Provides the corresponding icon in the right side of the list view.
 * Setter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE.
 * @param accessoryType One of "none", "hasChildren", "hasDetails" or "isChecked".
 * @return One of the following constants:
 * - MAW_RES_OK if the accessory type was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the accessoryType param was invalid.
 */
-(int) setAccessoryTypeProperty:(NSString*) accessoryType
{
    UITableViewCell *cell = self.cell;

    if ([accessoryType isEqualToString:@"hasChildren"])
    {
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    }
    else if ([accessoryType isEqualToString:@"hasDetails"])
    {
        cell.accessoryType = UITableViewCellAccessoryDetailDisclosureButton;
    }
    else if ([accessoryType isEqualToString:@"isChecked"])
    {
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
    }
    else if ([accessoryType isEqualToString:@"none"])
    {
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    return MAW_RES_OK;
}

/**
 * Set the font color of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_COLOR.
 * @param fontColor  A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return One of the following constants:
 * - MAW_RES_OK if the font color was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font color param was invalid.
 */
-(int) setFontColorProperty:(NSString*) fontColor
{
    UILabel* label = self.cell.textLabel;
    UIColor* color = [UIColor colorWithHexString:fontColor];
    if (!color)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    label.textColor = color;
    return MAW_RES_OK;
}

/**
 * Set the font size of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_SIZE.
 * @param fontSize  Font size, in points.
 * @return One of the following constants:
 * - MAW_RES_OK if the font size was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font size param was invalid.
 */
-(int) setFontSizeProperty:(NSString*) fontSize
{
    if (![fontSize canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    float fontSizeFloat = [fontSize floatValue];
    UILabel* label = self.cell.textLabel;
    UIFont* currentFont = label.font;
    NSString* currentFontName = [currentFont fontName];
    UIFont* newFont = [UIFont fontWithName:currentFontName size:fontSizeFloat];
    [label setFont:newFont];
    [self layout];
    return MAW_RES_OK;
}

/**
 * Set the font handle of the text
 * Setter for MAW_LIST_VIEW_ITEM_FONT_HANDLE.
 * @param fontHandle A font handle received from loading fonts using
 * maFontGetName and maFontLoadWithName syscalls.
 * @return One of the following constants:
 * - MAW_RES_OK if the font handle was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the font handle param was invalid.
 */
-(int) setFontHandleProperty:(NSString*) fontHandle
{
    if (![fontHandle canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UILabel* label = self.cell.textLabel;
    UIFont* font = Base::getUIFontObject([fontHandle intValue]);
    if (!font)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    [label setFont:font];
    [self layout];
    return MAW_RES_OK;
}

/**
 * Set the widget's background color.
 * Setter for MAW_WIDGET_BACKGROUND_COLOR.
 * @param backgroundColor  A hexadecimal value 0xRRGGBB, where R, G and B are the red, green and blue
 * components respectively.
 * @return One of the following constants:
 * - MAW_RES_OK if the background color was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the background color param was invalid.
 */
-(int) setBackgroundColorProperty:(NSString*) backgroundColor
{
    if (![backgroundColor canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    UIColor* color = [UIColor colorWithHexString:backgroundColor];
    if (!color)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.cell.contentView.backgroundColor = color;
    self.cell.textLabel.backgroundColor = color;
    self.cell.accessoryView.backgroundColor = color;
    return MAW_RES_OK;
}

/**
 * Set cell editable property value.
 * Setter for MAW_LIST_VIEW_ITEM_EDIT.
 * @param value "true" or "false" values.
 * @return MAW_RES_OK if value param is valid, or MAW_RES_INVALID_PROEPRTY_VALUE otherwise.
 */
-(int) setEditableProperty:(NSString*) value
{
    if (![value isEqualToString:kWidgetFalseValue] &&
        ![value isEqualToString:kWidgetTrueValue])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.editable = [value boolValue];
    return MAW_RES_OK;
}

/**
 * Check if cell is editable.
 * Setter for MAW_LIST_VIEW_ITEM_EDIT.
 * @return "true" is cell is editable, "false" otherwise.
 */
-(NSString*) isEditableProperty
{
    NSString* returnValue = self.editable ? kWidgetTrueValue : kWidgetFalseValue;
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
    if (![value isEqualToString:kWidgetTrueValue]    &&
        ![value isEqualToString:kWidgetFalseValue]   &&
        ![animated isEqualToString:kWidgetTrueValue] &&
        ![animated isEqualToString:kWidgetFalseValue])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [self.cell setSelected:[value boolValue] animated:[animated boolValue]];
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
    if (![value isEqualToString:kWidgetTrueValue]    &&
        ![value isEqualToString:kWidgetFalseValue]   &&
        ![animated isEqualToString:kWidgetTrueValue] &&
        ![animated isEqualToString:kWidgetFalseValue])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [self.cell setHighlighted:[value boolValue] animated:[animated boolValue]];
    return MAW_RES_OK;
}

/**
 * Set the type of standard accessory used by the item(normal state).
 * Setter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT.
 * @param accessoryTypeString One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @param state If YES, the accessory will be set for normal mode, otherwise it will be set for
 * editing mode.
 * @return MAW_RES_OK if the accessoryTypeString is valid, otherwise MAW_RES_INVALID_PROEPRTY_VALUE.
 */
-(int) setAccessoryTypeIntProperty:(NSString*) accessoryTypeString
                       normalState:(BOOL) state
{
    if (![self isAccessoryTypeIntValid:accessoryTypeString])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }
    UITableViewCellAccessoryType type = [self getAccessoryTypeIntFromString:accessoryTypeString];
    if (state)
    {
        self.cell.accessoryType = type;
    }
    else
    {
        self.cell.editingAccessoryType = type;
    }
    return MAW_RES_OK;
}

/**
 * Get the string constant assocciated with an accessory type.
 * Getter for MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_INT.
 * @param accessoryType A cell accessory type.
 * @return One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 */
-(NSString*) getAccessoryTypeIntProperty:(UITableViewCellAccessoryType) accessoryType
{
    int accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
    switch (accessoryType)
    {
        case UITableViewCellAccessoryDisclosureIndicator:
            accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE;
            break;
        case UITableViewCellAccessoryDetailDisclosureButton:
            accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL;
            break;
        case UITableViewCellAccessoryCheckmark:
            accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK;
            break;
        case UITableViewCellAccessoryNone:
        default:
            accessoryTypeInt = MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE;
            break;
    }

    return [NSString stringWithFormat:@"%d", accessoryTypeInt];
}

/**
 * Get the cell accessory type assocciated with a string constant.
 * @param accessoryTypeString One of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE constants.
 * @return A cell accessory type. If the accessoryTypeString is invalid UITableViewCellAccessoryNone
 * is returned.
 */
-(UITableViewCellAccessoryType) getAccessoryTypeIntFromString:(NSString*) accessoryTypeString
{
    UITableViewCellAccessoryType accessoryType =  UITableViewCellAccessoryNone;
    int accessoryTypeInt = [accessoryTypeString intValue];
    switch (accessoryTypeInt)
    {
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
            accessoryType = UITableViewCellAccessoryDisclosureIndicator;
            break;
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
            accessoryType = UITableViewCellAccessoryDetailDisclosureButton;
            break;
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
            accessoryType = UITableViewCellAccessoryCheckmark;
            break;
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
        default:
            accessoryType = UITableViewCellAccessoryNone;
            break;
    }
    return accessoryType;
}

/**
 * Check if a given string contains one of the MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE values.
 * @return YES if it contains, NO otherwise.
 */
-(BOOL) isAccessoryTypeIntValid:(NSString*) accessoryTypeString
{
    if (![accessoryTypeString canParseNumber])
    {
        return NO;
    }
    BOOL returnValue = NO;
    switch ([accessoryTypeString intValue])
    {
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DISCLOSURE:
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_DETAIL:
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_CHECKMARK:
        case MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE_NONE:
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
 * Setter for MAW_LIST_VIEW_ITEM_EDITING_STYLE.
 * @param editingStyleString One of the MAW_LIST_VIEW_ITEM_EDIT_STYLE constants.
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
        case MAW_LIST_VIEW_ITEM_EDIT_STYLE_DELETE:
            style = UITableViewCellEditingStyleDelete;
            break;
        case MAW_LIST_VIEW_ITEM_EDIT_STYLE_INSERT:
            style = UITableViewCellEditingStyleInsert;
            break;
        case MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE:
            style = UITableViewCellEditingStyleNone;
            break;
        default:
            returnValue = MAW_RES_INVALID_PROPERTY_VALUE;
            break;
    }
    if (returnValue == MAW_RES_OK)
    {
        self.editingStyle = style;
    }

    return returnValue;
}

/**
 * Get the editing style.
 * Getter for MAW_LIST_VIEW_ITEM_EDITING_STYLE.
 * @return One of the MAW_LIST_VIEW_ITEM_EDIT_STYLE constants.
 */
-(NSString*) getEditingStyleProperty
{
    int style;
    switch (self.cell.editingStyle)
    {
        case UITableViewCellEditingStyleDelete:
            style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_DELETE;
            break;
        case UITableViewCellEditingStyleInsert:
            style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_INSERT;
            break;
        case UITableViewCellEditingStyleNone:
        default:
            style = MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE;
            break;
    }
    return [NSString stringWithFormat:@"%d", style];
}

/**
 * Set the selection style.
 * Setter for MAW_LIST_VIEW_ITEM_SELECTION_STYLE.
 * @param selectionStyleString One of the MAW_LIST_VIEW_ITEM_SELECTION_STYLE constants.
 * @return MAW_RES_OK if the selectionStyleString is valid, otherwise MAW_RES_INVALID_PROPERTY_VALUE.
 */
-(int) setSelectionStyleProperty:(NSString*) selectionStyleString
{
    // Check if param can be parsed into number.
    if (![selectionStyleString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UITableViewCellSelectionStyle style;
    BOOL isSelectionStyleParamValid = YES;

    // Convert int param into UITableViewCellSelectionStyle type.
    switch ([selectionStyleString intValue])
    {
        case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_BLUE:
            style = UITableViewCellSelectionStyleBlue;
            break;
        case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_GRAY:
            style = UITableViewCellSelectionStyleGray;
            break;
        case MAW_LIST_VIEW_ITEM_SELECTION_STYLE_NONE:
            style = UITableViewCellSelectionStyleNone;
            break;
        default:
            isSelectionStyleParamValid = NO;
            break;
    }

    // Check if selectionStyleString param is valid.
    if (!isSelectionStyleParamValid)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    // Set the style.
    self.cell.selectionStyle = style;
    return MAW_RES_OK;
}

/**
 * Get the selection style.
 * Getter for MAW_LIST_VIEW_ITEM_SELECTION_STYLE.
 * @return One of the MAW_LIST_VIEW_ITEM_SELECTION_STYLE constants.
 */
-(NSString*) getSelectionStyleProperty
{
    int selectionStyleInt;

    switch (self.cell.selectionStyle)
    {
        case UITableViewCellSelectionStyleGray:
            selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_GRAY;
            break;
        case UITableViewCellSelectionStyleNone:
            selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_NONE;
            break;
        case UITableViewCellSelectionStyleBlue:
        default:
            selectionStyleInt = MAW_LIST_VIEW_ITEM_SELECTION_STYLE_BLUE;
            break;

    }

    return [NSString stringWithFormat:@"%d", selectionStyleInt];
}

@end