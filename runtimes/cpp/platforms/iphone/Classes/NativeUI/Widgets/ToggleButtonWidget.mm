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
 * @file ToggleButtonWidget.mm
 * @author Bogdan Iusco
 * @date 27 Sep 2012
 *
 * @brief Implement NativeUI ToggleButton widget on iOS platform.
 * It is the same as the CheckBox widget, so the class will be inherited
 * from CheckBoxWidget.
 */

#import "ToggleButtonWidget.h"

@implementation ToggleButtonWidget

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
    NSString* togglePropertyName = key;
    if ([key isEqualToString:@MAW_TOGGLE_BUTTON_CHECKED])
    {
        key = @MAW_CHECK_BOX_CHECKED;
    }
    return [super setPropertyWithKey:togglePropertyName toValue:value];
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    NSString* togglePropertyName = key;
    if ([key isEqualToString:@MAW_TOGGLE_BUTTON_CHECKED])
    {
        key = @MAW_CHECK_BOX_CHECKED;
    }
    return [super getPropertyWithKey:togglePropertyName];
}

@end
