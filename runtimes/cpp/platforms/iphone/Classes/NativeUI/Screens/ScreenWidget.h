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

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key;

/**
 * Check if a given child screen is shown inside this screen.
 * This function should be implemented by all classed inherited from this class,
 * such as StackScreenWidget and TabScreenWidget.
 * @param childScreen Screen to check.
 * @return This function returns NO.
 */
- (BOOL)isChildScreenShown:(ScreenWidget*)childScreen;

/**
 * Check if the screen is shown.
 * @return "true" is the screen is shown, "false" otherwise.
 */
- (NSString*)isShownProperty;

@end
