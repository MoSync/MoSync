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

@interface StackScreenWidget : ScreenWidget <UINavigationControllerDelegate>
{
    NSMutableArray* stack;
}

/**
 * Pop the top view controller from the navigation stack.
 * Called by the UINavigationController object.
 * For more info see UINavigationControllerExpanded file.
 */
- (void)viewControllerWillBePoped;

/**
 * Push a view controller on the navigation stack.
 * @param child Widget containing a view controller(e.g. ScreenWidget).
 */
- (void)push:(IWidget*)child;

/**
 * Pop the top screen from the stack.
 */
- (void)pop;

/**
 * Recalculate its and children size.
 */
- (void)layout;

/**
 * Set a widget property value.
 * @param key Widget's property name that should be set.
 * @param value Widget's proeprty value that should be set.
 * @return One of the following values:
 * - MAW_RES_OK if the property was set.
 * - MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 */
- (int)setPropertyWithKey: (NSString*)key toValue:(NSString*)value;

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

/**
 * Check if a given child screen is shown inside this stack screen.
 * @param childScreen Screen to check.
 * @return YES if the childScreen is currently shown, NO otherwise.
 */
- (BOOL)isChildScreenShown:(ScreenWidget*)childScreen;

@end
