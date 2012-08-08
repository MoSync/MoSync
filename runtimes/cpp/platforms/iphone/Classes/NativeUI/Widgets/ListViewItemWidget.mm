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

//#include "Platform.h"
//#include <helpers/cpp_defs.h>
//#include <helpers/CPP_IX_WIDGET.h>
//#include <base/Syscall.h>

#import "ListViewItemWidget.h"
#import "UIColorExpanded.h"
#import "HorizontalLayoutWidget.h"
#import "VerticalLayoutWidget.h"
#import "RelativeLayoutWidget.h"

@implementation ListViewItemWidget

static NSString* kReuseIdentifier = @"Cell";

@synthesize deleteButtonTitle = _deleteButtonTitle;
@synthesize editingStyle = _editingStyle;
@synthesize editable = _canEdit;
@synthesize canMove = _canMove;

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UITableViewCell* cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault
                                       reuseIdentifier:kReuseIdentifier];
        self.view = cell;
        [cell release];
        cell = nil;
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
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not an layout.
 */
- (int)addChild:(IWidget*)child
{
    if ([child class] != [HorizontalLayoutWidget class] &&
        [child class] != [VerticalLayoutWidget class] &&
        [child class] != [RelativeLayoutWidget class] &&
        [child superclass] != [HorizontalLayoutWidget class] &&
        [child superclass] != [VerticalLayoutWidget class] &&
        [child superclass] != [RelativeLayoutWidget class])
    {
        return MAW_RES_INVALID_LAYOUT;
    }
    [super addChild:child toSubview:YES];
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if the child's type is not an layout.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
    if ([child class] != [HorizontalLayoutWidget class] &&
        [child class] != [VerticalLayoutWidget class] &&
        [child class] != [RelativeLayoutWidget class] &&
        [child superclass] != [HorizontalLayoutWidget class] &&
        [child superclass] != [VerticalLayoutWidget class] &&
        [child superclass] != [RelativeLayoutWidget class])
    {
        return MAW_RES_INVALID_LAYOUT;
    }

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
    if([key isEqualToString:@MAW_LIST_VIEW_ITEM_TEXT]) {
		UITableViewCell* cell = (UITableViewCell*) self.view;
		UILabel* label = cell.textLabel;
		label.text = value;
		[self layout];
	}
    else if ([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_HANDLE])
    {
        UITableViewCell* cell = (UITableViewCell*) self.view;
		UILabel* label = cell.textLabel;
        UIFont* font = Base::getUIFontObject([value intValue]);
        if (!font)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        [label setFont:font];
        [self layout];
    }
	else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_ICON]) {
		int imageHandle = [value intValue];
		if(imageHandle<=0) return MAW_RES_INVALID_HANDLE;
		UITableViewCell* cell = (UITableViewCell*) self.view;
		UIImageView* imageView = cell.imageView;
		Surface* imageResource = Base::gSyscall->resources.get_RT_IMAGE(imageHandle);
		imageView.image = [UIImage imageWithCGImage:imageResource->image];
	}
	else if([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR]) {
		UITableViewCell *cell = (UITableViewCell*)self.view;
		UIColor* color = [UIColor colorWithHexString:value];
		if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;

		cell.contentView.backgroundColor = color;
		cell.textLabel.backgroundColor = color;
		cell.accessoryView.backgroundColor = color;
	}
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE]) {
		UITableViewCell *cell = (UITableViewCell*)self.view;

		if([value isEqualToString:@"hasChildren"]) {
			cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
		}
		else if([value isEqualToString:@"hasDetails"]) {
			cell.accessoryType = UITableViewCellAccessoryDetailDisclosureButton;
		}
		else if([value isEqualToString:@"isChecked"]) {
			cell.accessoryType = UITableViewCellAccessoryCheckmark;
		}
		else if([value isEqualToString:@"none"]) {
			cell.accessoryType = UITableViewCellAccessoryNone;
		} else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
	}
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_COLOR])
    {
        UITableViewCell* cell = (UITableViewCell*) self.view;
		UILabel* label = cell.textLabel;
        UIColor* color = [UIColor colorWithHexString:value];
		if(!color)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        label.textColor = color;
    }
    else if([key isEqualToString:@MAW_LIST_VIEW_ITEM_FONT_SIZE])
    {
        UITableViewCell* cell = (UITableViewCell*) self.view;
		UILabel* label = cell.textLabel;
		float fontSize = [value floatValue];
        UIFont* currentFont = label.font;
        NSString* currentFontName = [currentFont fontName];
        UIFont* newFont = [UIFont fontWithName:currentFontName size:fontSize];
        [label setFont:newFont];
		[self layout];
    }
	else {
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
    return [super getPropertyWithKey:key];
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
