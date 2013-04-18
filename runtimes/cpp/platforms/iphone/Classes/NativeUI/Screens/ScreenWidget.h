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
#import "IWidget.h"
#import "ScreenWidgetController.h"

@interface ScreenWidget : IWidget
{
    /**
     * View's controller.
     */
	UIViewController* _controller;
}

/**
 * Init function.
 * A view controller will be created for you.
 */
- (id)init;

/**
 * Init function.
 * @param controller Controller for the view.
 */
- (id)initWithController:(UIViewController*)controller;

/**
 * Adds an widget to the end of the children list.
 * A ScreenWidget can have only one child.
 * @param child Widget to be added.
 * @return MAW_RES_OK for success, MAW_RES_ERROR if the screen already has an child.
 */
- (int)addChild:(IWidget*)child;

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index Screen widget can have only one child, so this value must be zero.
 * @return One of the following values:
 * - MAW_RES_OK if the child could be added to the parent.
 * - MAW_RES_INVALID_INDEX if the index param is not zero.
 */
- (int)insertChild: (IWidget*)child atIndex:(NSNumber*)index;

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child;

/**
 * Set child's size.
 */
-(void) layout;

/**
 * Get view's controller.
 * @return View's controller.
 */
- (UIViewController*) getController;

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

@end
