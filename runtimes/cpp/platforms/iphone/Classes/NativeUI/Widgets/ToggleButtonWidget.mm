/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
