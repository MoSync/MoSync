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
 * @file ListViewSectionWidget.h
 * @author Bogdan Iusco
 * @date 07 Aug 2012
 *
 * @brief ListViewSectionWidget widget interface file.
 * A ListViewSectionWidget object wraps one or more ListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */

#import <Foundation/Foundation.h>

#import "IWidget.h"
#import "ListViewWidgetDelegate.h"
#import "ListViewSectionWidgetDelegate.h"

// Forward declaration.
@class ListViewItemWidget;

/**
 * @brief A ListViewSection object wraps one or more ListViewItemWidget
 * objects and their characteristics(such as header and footer text).
 */
@interface ListViewSectionWidget : IWidget<ListViewSectionWidgetDelegate>
{
    /**
     * Delegate to the list that has this section.
     */
    id <ListViewWidgetDelegate> _delegate;

    /**
     * Section's title displayed on the right side of the table view.
     * The table view must be in the plain style.
     */
    NSString* _title;

    /**
     * A string used as the title of the section header.
     */
    NSString* _headerTitle;

    /**
     * A string used as the title of the section footer.
     */
    NSString* _footerTitle;

    /**
     * Section's index in the table view.
     */
    NSInteger _index;
}

@property (retain) id delegate;
@property(nonatomic, retain) NSString* title;
@property(nonatomic, retain) NSString* headerTitle;
@property(nonatomic, retain) NSString* footerTitle;

/**
 * Setter and getter for section's index in the table view.
 */
@property(nonatomic, assign) NSInteger index;

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not ListViewItemWidget.
 */
- (int)addChild:(IWidget*)child;

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not ListViewItemWidget.
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
 * Return the number of contained cells.
 */
-(NSInteger) countCells;

/**
 * Remove a given cell from section.
 * @param cell Cell to be removed.
 */
-(void) removeCell:(ListViewItemWidget*) cell;

/**
 * Get a cell at a given index.
 * @param index Cell's index.
 * @return Cell at the given index if the index is valid, nil otherwise.
 */
-(ListViewItemWidget*) cellWidgetAtIndex:(NSInteger) index;

@end
