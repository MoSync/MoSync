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

@end
