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
 * @file ListViewItemWidget.h
 * @author Bogdan Iusco
 *
 * @brief ListViewItemWidget widget interface file.
 * A ListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in ListViewWidget objects.
 */

#import <Foundation/Foundation.h>

#import "IWidget.h"
#import "ListViewSectionWidgetDelegate.h"

/**
 * @brief ListViewItemWidget widget interface file.
 * A ListViewItemWidget object is used to define the attributes and behaviour of the
 * cells/items that appear in ListViewWidget objects.
 */
@interface ListViewItemWidget : IWidget
{

    /**
     * Delegate to the section that the item belongs.
     */
    id <ListViewSectionWidgetDelegate> _delegate;

    /**
     * The title of the delete-confirmation button.
     * The table view displays a button with this text when the user attempts to delete a row,
     * either by swiping the row or tapping the red minus icon in editing mode.
     */
    NSString* _deleteButtonTitle;

    /**
     * Cell's editing style.
     */
    UITableViewCellEditingStyle _editingStyle;

    /**
     * Flag for cell editing property.
     */
    BOOL _canEdit;
}

/**
 * Delegate property.
 */
@property (retain) id delegate;

/**
 * Get item's cell.
 */
@property(nonatomic, readonly, getter = cell) UITableViewCell* cell;

/**
 * Getter and setter for the title of the delete-confirmation button.
 * The table view displays a button with this text when the user attempts to delete a row,
 * either by swiping the row or tapping the red minus icon in editing mode.
 */
@property(nonatomic, retain) NSString* deleteButtonTitle;

/**
 * Getter and setter for cell's editing style.
 */
@property(nonatomic, assign) UITableViewCellEditingStyle editingStyle;

/**
 * Getter and setter for editable property.
 */
@property(nonatomic, assign) BOOL editable;

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * It is recommended that the child should be an layout.
 * @return MAW_RES_OK.
 */
- (int)addChild:(IWidget*)child;

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * It is recommended that the child should be an layout.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index;

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child;

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
 * Asks the widget to calculate and return the size that best fits its subviews.
 * @return The size that best fits itself.
 */
-(CGSize)sizeThatFitsForWidget;

/**
 * Recalculate its and children size.
 */
- (void)layout;

@end
