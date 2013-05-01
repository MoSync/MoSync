/* Copyright (C) 2013 MoSync AB

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
 * @file CustomPickerWidget.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI CustomPicker widget.
 * UIPickerView with layout widgets as items.
 */

#import "IWidget.h"

/**
 * @brief NativeUI CustomPicker widget.
 * The widget is implementat as a UIPickerView.
 */
@interface CustomPickerWidget : IWidget<UIPickerViewDataSource, UIPickerViewDelegate>

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added. Its type must be HorizontalLayout,
 * RelativeLayout or VerticalLayout.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not as expected.
 */
- (int)addChild:(IWidget*)child;

/**
 * Insert a widget to a given index.
 * @param child Widget to be added. Its type must be HorizontalLayout,
 * RelativeLayout or VerticalLayout.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT If its type is not as expected.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index;

/**
 * Remove a child from the widget.
 * The child's view will be removed from its superview.
 * @param child Widget to remove.
 */
- (void)removeChild:(IWidget*)child;

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value;

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

@end
