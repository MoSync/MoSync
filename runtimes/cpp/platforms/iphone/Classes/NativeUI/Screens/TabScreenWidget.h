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

#import <Foundation/Foundation.h>
#import "ScreenWidget.h"

@interface TabScreenWidget : ScreenWidget <UITabBarControllerDelegate>

/**
 * Adds an screen widget to the tab screen.
 * @param screen Screen to be added.
 * @return One of the following constants:
 * - MAW_RES_OK if the screen was added.
 * - MAW_RES_ERROR if the screen param is not ScreenWidget or derived from it.
 */
- (int)addChild:(IWidget*)screen;

/**
 * Insert a screen to a given index.
 * @param screen Widget to be added.
 * @param index The index where the screen should be inserted.
 * - MAW_RES_OK if the screen was added.
 * - MAW_RES_INVALID_INDEX if the index param is invalid.
 * - MAW_RES_ERROR if the screen param is not ScreenWidget or derived from it.
 */
- (int)insertChild: (IWidget*)screen atIndex:(NSNumber*)index;

/**
 * Remove a screen from the tab screen.
 * @param screen Screen to be removed.
 */
- (void)removeChild: (IWidget*)screen;

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

/**
 * Recalculate its and children size.
 */
- (void)layout;

/**
 * Check if a given child screen is shown inside this tab screen.
 * @param childScreen Screen to check.
 * @return YES if the childScreen is currently shown, NO otherwise.
 */
- (BOOL)isChildScreenShown:(ScreenWidget*)childScreen;

@end
