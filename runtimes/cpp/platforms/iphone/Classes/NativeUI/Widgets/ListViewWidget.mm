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

/**
 * @file ListViewWidget.mm
 * @author Bogdan Iusco
 *
 * @brief ListView widget interface file.
 * A ListView widget is used to present an indexed/grouped list of items.
 */

/**
 * Default title for the delete-confirmation button, which appears on the right side of the cell.
 */
#define DEFAULT_DELETE_BUTTON_TITLE "Delete";

#include "Platform.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "ListViewWidget.h"
#import "ListViewSectionWidget.h"
#import "ListViewItemWidget.h"
#import "NSStringExpanded.h"
#import "UIColorExpanded.h"

@implementation ListViewItemAnimation

@synthesize type = _type;
@synthesize indexPath = _indexPath;

- (id)init
{
    self = [super init];
    if (self)
    {
        _type = ListViewItemAnimationTypeNone;
        _indexPath = nil;
    }

    return self;
}

- (void) dealloc
{
    if (_indexPath != nil)
    {
        [_indexPath release];
        _indexPath = nil;
    }

    [super dealloc];
}

@end

/**
 * Private methods for ListViewWidget.
 */
@interface ListViewWidget (hidden)

/**
 * Create a UITableView object and set the delegates.
 * If the list already has an view object it will be released and
 * a new one will be created.
 * The newest table view style will be set accordingly to the list view's type.
 * All its children will be realeased.
 */
-(void)createTableView;

/**
 * Set the list view type.
 * Setter for MAW_LIST_VIEW_TYPE.
 * All its children will be deleted.
 * Will create a new UITableView object.
 * @param listType Must be one of the following values:
 * - MAW_LIST_VIEW_TYPE_DEFAULT
 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
 * - MAW_LIST_VIEW_TYPE_SEGMENTED
 * @return An string object containing one of the following values:
 * - MAW_RES_OK if the list's type was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the listType param was invalid.
 */
-(int)setListViewTypeProperty:(NSString*)listType;

/**
 * Get the list view type property.
 * Getter for MAW_LIST_VIEW_TYPE.
 * @return An NSString object containg the following valus:
 * - MAW_LIST_VIEW_TYPE_DEFAULT
 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
 * - MAW_LIST_VIEW_TYPE_SEGMENTED
 */
-(NSString*)getListViewTypeProperty;

/**
 * Set the list view editing mode.
 * Setter for MAW_LIST_VIEW_MODE.
 * @param mode MAW_LIST_VIEW_MODE_DISPLAY will turn the editing mode to off, or
 * MAW_LIST_VIEW_MODE_EDIT will allow editing.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the mode param was invalid.
 */
-(int)setModeProperty:(NSString*) mode;

/**
 * Check if the list is in editing mode or not.
 * Getter for MAW_LIST_VIEW_MODE.
 * @return One of the following values:
 * - MAW_LIST_VIEW_MODE_DISPLAY if the list in not in the editing mdoe.
 * - MAW_LIST_VIEW_MODE_EDIT if the list is in the editing mode.
 */
-(NSString*) getModeProperty;

/**
 * Set if the list view should indent ListViewItems with
 * the edit style set to MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE.
 * Setter for MAW_LIST_VIEW_INDENT_WHILE_EDITING.
 * @param shouldIndent "true" if the items should be indented, "false" if they should not
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the shouldIndent param was invalid.
 */
-(int)setIndentWhileEditingProperty:(NSString*) shouldIndent;

/**
 * Check if the list view indents items with the style
 * MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE set.
 * Getter for MAW_LIST_VIEW_INDENT_WHILE_EDITING.
 * @return "true" if the list indents MAW_LIST_VIEW_ITEM_EDIT_STYLE_NONE items, "false" otherwise.
 */
-(NSString*) getIndentWhileEditingProperty;

/**
 * Enable/disable user interaction with an list view item.
 * A boolean value that determines whether the users can select a row will be set.
 * Setter for MAW_LIST_VIEW_ALLOW_SELECTION.
 * @param allowSelection "true" will allow the user to select an item, "false" will not
 * allow the user to select it.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the allowSelection param was invalid.
 */
-(int)setAllowSelectionProperty:(NSString*) allowSelection;

/**
 * Check if user can select an list view item.
 * Getter for MAW_LIST_VIEW_ALLOW_SELECTION.
 * @return "true" if the user can select an item, "false" otherwise.
 */
-(NSString*) getAllowSelectionProperty;

/**
 * Enable/disable user interaction with an list view item in edit mode.
 * A boolean value that determines whether the users can select a row will be set.
 * Setter for MAW_LIST_VIEW_ALLOW_SELECTION_DURING_EDITING.
 * @param allowSelection "true" will allow the user to select an item, "false" will not
 * allow the user to select it.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the allowSelection param was invalid.
 */
-(int)setAllowSelectionDuringEditingProperty:(NSString*) allowSelection;

/**
 * Check if user can select an list view item in edit mode.
 * Getter for MAW_LIST_VIEW_ALLOW_SELECTION_DURING_EDITING.
 * @return "true" if the user can select an item, "false" otherwise.
 */
-(NSString*) getAllowSelectionDuringEditingProperty;

/**
 * Enable/disable user scrolling with a list view.
 * A boolean value that determines whether the users can scroll the list.
 * Setter for MAW_LIST_VIEW_ALLOW_SCROLLING.
 * @param allowScrolling "true" will allow the user to scroll the list, "false" will not
 * allow the user to select it.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the allowSelection param was invalid.
 */
-(int)setAllowScrollingProperty:(NSString*) allowScrolling;

/**
 * Check if user can scroll a list view.
 * Getter for MAW_LIST_VIEW_ALLOW_SCROLLING.
 * @return "true" if the user can scroll the list, "false" otherwise.
 */
-(NSString*) getAllowScrollingProperty;

/**
 * Replaces the list view background with
 * a single color background.
 * @param colorValue A color in hex
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the colorValue could not be parsed as a valid color.
 */
-(int)setBackgroundColor:(NSString*) colorValue;

/**
 * Starts or stops the animation batch
 * @param value "true" if the animation batch should start,
 * "false" if the animation batch is done and the animations should start.
 * @return Currently, MAW_RES_OK is the only return value.
 */
-(int) setAnimationState:(NSString*) value;

/**
 * Calculates and stores the proper indexPath depending on the other animation batch operations
 * currently being played at the same time.
 * @param type Type of animation (insert, delete, etc)
 * @param indexPath Index path of the current data source to do the operation on
 */
-(void)createAnimationType:(ListViewItemAnimationType)type itemPath:(NSIndexPath*) indexPath;

@end

@implementation ListViewWidget

@synthesize type = _type;
@synthesize shouldIndentWhileEditing = _shouldIndentWhileEditing;

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _type = ListViewTypeDefault;
        _shouldIndentWhileEditing = YES;
        _isAnimating = NO;
        _itemOperations = [[NSMutableArray alloc] init];

        [self createTableView];
    }

    return self;
}

/**
 * Getter for the widget's view.
 */
-(UITableView*) tableView
{
    UITableView* tableView = (UITableView*) self.view;
    return tableView;
}

/**
 * Adds an widget to the end of the children list.
 * @param child Widget to be added.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT if child's type is not as expected.
 * If list view's type is ListViewTypeDefault then the child's type must be
 * ListViewItemWidget, otherwise its type must be ListViewSectionWidget.
 */
- (int)addChild:(IWidget*)child
{
    if (_type == ListViewTypeDefault)
    {
      if ([child class] != [ListViewItemWidget class] &&
        [child superclass] != [ListViewItemWidget class])
      {
        return MAW_RES_INVALID_LAYOUT;
      }
      else
      {
          ListViewSectionWidget* section = (ListViewSectionWidget*)[_children objectAtIndex:0];
          [section addChild:child];
      }
    }
    else if (_type == ListViewTypeAlphabetical ||
             _type == ListViewTypeSegmented)
    {
        if ([child class] != [ListViewSectionWidget class] &&
            [child superclass] != [ListViewSectionWidget class])
        {
            return MAW_RES_INVALID_LAYOUT;
        }
        else
        {
            [super addChild:child toSubview:NO];
            ListViewSectionWidget* section = (ListViewSectionWidget*) child;
            section.index = [_children count] - 1;
            section.delegate = self;
        }
    }
    return MAW_RES_OK;
}

/**
 * Insert a widget to a given index.
 * @param child Widget to be added.
 * @param index The index where the child should be inserted.
 * @return One of the following constants:
 * - MAW_RES_OK if the child was added.
 * - MAW_RES_INVALID_LAYOUT If list view's type is ListViewTypeDefault then the child's
 * type must be ListViewItemWidget, otherwise its type must be ListViewSectionWidget.
 * - MAW_RES_INVALID_INDEX if the index was out of bounds.
 */
- (int)insertChild:(IWidget*)child atIndex:(NSNumber*)index
{
    int result = MAW_RES_OK;
    if (_type == ListViewTypeDefault)
    {
        if ([child class] != [ListViewItemWidget class] &&
            [child superclass] != [ListViewItemWidget class])
        {
            result = MAW_RES_INVALID_LAYOUT;
        }
        else
        {
            ListViewSectionWidget* section = (ListViewSectionWidget*)[_children objectAtIndex:0];
            result = [section insertChild:child atIndex:index];
        }
    }
    else if (_type == ListViewTypeAlphabetical ||
             _type == ListViewTypeSegmented)
    {
        if ([child class] != [ListViewSectionWidget class] &&
            [child superclass] != [ListViewSectionWidget class])
        {
            result = MAW_RES_INVALID_LAYOUT;
        }
        else
        {
            result = [self insertChild:child atIndex:index toSubview:NO];
        }
    }
    return result;
}

/**
 * Remove a child from the widget.
 * The child's view will be removed its superview.
 * @param child Widget to remove.
 */
- (void)removeChild: (IWidget*)child
{
    int childIndex = -1;
    if (_type == ListViewTypeDefault)
    {
        ListViewSectionWidget* section = (ListViewSectionWidget*)[_children objectAtIndex:0];
        childIndex = [section indexOfChild:child];
        [section removeChild:child];
    }
    else
    {
        [_children removeObjectIdenticalTo:child];
        [child setParent:nil];
    }

    if (!_isAnimating)
    {
        UITableView* tableView = (UITableView*) self.view;
        [tableView reloadData];
    }
}

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
    int returnValue = MAW_RES_OK;

    if ([key isEqualToString:@MAW_LIST_VIEW_TYPE])
    {
        returnValue = [self setListViewTypeProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_MODE])
    {
        returnValue = [self setModeProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_INDENT_WHILE_EDITING])
    {
        returnValue = [self setIndentWhileEditingProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SELECTION])
    {
        returnValue = [self setAllowSelectionProperty:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SELECTION_DURING_EDITING])
    {
        returnValue = [self setAllowSelectionDuringEditingProperty:value];
    }
    else if ([key isEqualToString:@MAW_WIDGET_BACKGROUND_COLOR])
    {
        returnValue = [self setBackgroundColor:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_BEGIN_ANIMATION])
    {
        returnValue = [self setAnimationState:value];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SCROLLING])
    {
        returnValue = [self setAllowScrollingProperty:value];
    }
    else
    {
        returnValue = [super setPropertyWithKey:key toValue:value];
    }

    return returnValue;
}

/**
 * Get a widget property value.
 * @param key Widget's property name.
 * @return The property value, or nil if the property name is invalid.
 * The returned value should not be autoreleased. The caller will release the returned value.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    if ([key isEqualToString:@MAW_LIST_VIEW_TYPE])
    {
        return [[self getListViewTypeProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_MODE])
    {
        return [[self getModeProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_INDENT_WHILE_EDITING])
    {
        return [[self getIndentWhileEditingProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SELECTION])
    {
        return [[self getAllowSelectionProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SELECTION_DURING_EDITING])
    {
        return [[self getAllowSelectionDuringEditingProperty] retain];
    }
    else if ([key isEqualToString:@MAW_LIST_VIEW_ALLOW_SCROLLING])
    {
        return [[self getAllowScrollingProperty] retain];
    }
    else
    {
        return [super getPropertyWithKey:key];
    }
}

#pragma mark UITableViewDataSource

/**
 * Return the number of rows in a given section of a table view.
 * @param tableView The table-view object requesting this information.
 * @param section An index number identifying a section in tableView.
 * @return The number of rows in section.
 */
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    ListViewSectionWidget* tableSection = [_children objectAtIndex:section];
    if (!tableSection)
    {
        return 0;
    }

    return [tableSection countCells];
}

/**
 * Return the cell to insert in a particular location of the table view.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path locating a row in tableView.
 * @return The cell to insert at the given row.
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return nil;
    }

    ListViewItemWidget *cellWidget = [section cellWidgetAtIndex:indexPath.row];
    if (!cellWidget)
    {
        return nil;
    }
    return cellWidget.cell;
}

/**
 * Return the number of sections in the table view.
 * @param tableView The table-view object requesting this information.
 * @return the number of sections in the table view.
 */
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [_children count];
}

/**
 * Return the title of the header of the specified section of the table view.
 * @param tableView The table-view object requesting this information.
 * @param section An index number identifying a section of tableView.
 * @return A string to use as the title of the section header.
 */
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    ListViewSectionWidget* tableViewSection = [_children objectAtIndex:section];
    if (!tableViewSection)
    {
        return nil;
    }

    return tableViewSection.headerTitle;
}

/**
 * Return the title of the footer of the specified section of the table view.
 * @param tableView The table-view object requesting this information.
 * @param section An index number identifying a section of tableView.
 * @return A string to use as the title of the section footer.
 */
- (NSString *)tableView:(UITableView *)tableView titleForFooterInSection:(NSInteger)section
{
    ListViewSectionWidget* tableViewSection = [_children objectAtIndex:section];
    if (!tableViewSection)
    {
        return nil;
    }
    return tableViewSection.footerTitle;
}

/**
 * Verify that the given row is editable.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path locating a row in tableView.
 * @return YES if the row indicated by indexPath is editable; otherwise, NO.
 */
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return 0;
    }
    ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    return cellWidget.editable;
}

/**
 * Return the titles for the sections for a table view.
 * @param tableView The table-view object requesting this information.
 * @return An array of strings that serve as the title of sections in the table view and
 * appear in the index list on the right side of the table view.
 */
- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView
{
    NSMutableArray* array = [[[NSMutableArray alloc] init] autorelease];
    for (ListViewSectionWidget* section in _children)
    {
        if (section.title)
        {
            [array addObject:section.title];
        }
    }

    return array;
}

/**
 * Asks if to commit the insertion or deletion of a specified row in the receiver.
 * @param tableView The table view object requesting the insertion or deletion.
 * @param editingStyle he cell editing style corresponding to a insertion or deletion
 * requested for the row specified by indexPath.
 * @param indexPath An index path locating the row in tableView.
 */
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle
forRowAtIndexPath:(NSIndexPath *)indexPath
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;


    int widgetEventType = MAW_RES_ERROR;
    switch (editingStyle)
    {
        case UITableViewCellEditingStyleInsert:
            widgetEventType = MAW_EVENT_SEGMENTED_LIST_ITEM_INSERT_BTN;
            break;
        case UITableViewCellEditingStyleDelete:
            widgetEventType = MAW_EVENT_SEGMENTED_LIST_ITEM_DELETE_BTN;
            break;
        default:
            widgetEventType = MAW_RES_ERROR;
            break;
    }

    if (widgetEventType != MAW_RES_ERROR)
    {
        eventData->eventType = widgetEventType;
        eventData->sectionIndex = indexPath.section;
        eventData->sectionItemIndex = indexPath.row;

        eventData->widgetHandle = self.handle;
        event.data = (int)eventData;
        Base::gEventQueue.put(event);
    }
}

#pragma mark UITableViewDelegate

/**
 * Return the height to use for a row in a specified location.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path that locates a row in tableView.
 * @return A floating-point value that specifies the height (in points) that row should be.
 */
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return 0;
    }
    ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    return cellWidget.height;
}

/**
 * A specified row is about to be selected.
 * @param tableView A table-view object informing the delegate about the impending selection.
 * @param indexPath An index path locating the row in tableView.
 * @return An index-path object that confirms or alters the selected row.
 */
- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_ITEM_WILL_SELECT;
	eventData->widgetHandle = self.handle;
	eventData->listItemIndex = [indexPath row];
	event.data = (int)eventData;
	Base::gEventQueue.put(event);

    return indexPath;
}

/**
 * Called when a specified row is now selected.
 * @param tableView A table-view object informing the delegate about the new row selection.
 * @param indexPath An index path locating the row in tableView.
 */
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;

    if (_type == ListViewTypeDefault)
    {
        eventData->eventType = MAW_EVENT_ITEM_CLICKED;
        eventData->listItemIndex = [indexPath row];
    }
    else
    {
        eventData->eventType = MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED;
        eventData->sectionIndex = indexPath.section;
        eventData->sectionItemIndex = indexPath.row;
    }

	eventData->widgetHandle = self.handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

/**
 * Called when a specified row accessory is selected.
 * @param tableView A table-view object informing the delegate about the new row selection.
 * @param indexPath An index path locating the row in tableView.
 */
- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;

    if (_type == ListViewTypeDefault)
    {
        eventData->eventType = MAW_EVENT_LIST_ITEM_ACCESSORY_CLICKED;
        eventData->listItemIndex = [indexPath row];
    }
    else
    {
        eventData->eventType = MAW_EVENT_LIST_ITEM_ACCESSORY_CLICKED;
        eventData->sectionIndex = indexPath.section;
        eventData->sectionItemIndex = indexPath.row;
    }

	eventData->widgetHandle = self.handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

/**
 * Changes the default title of the delete-confirmation button.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index-path object locating the row in its section.
 * @return A localized string to used as the title of the delete-confirmation button.
 */
- (NSString *)tableView:(UITableView *)tableView titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return @DEFAULT_DELETE_BUTTON_TITLE;
    }
    ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    return cellWidget.deleteButtonTitle;
}

/**
 * Get the editing style of a row at a particular location in a table view.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path locating a row in tableView.
 * @return The editing style of the cell for the row identified by indexPath.
 */
- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return UITableViewCellEditingStyleNone;
    }
    ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    if (!cellWidget)
    {
        return UITableViewCellEditingStyleNone;
    }

    return cellWidget.editingStyle;
}

/**
 * Get the movable property of a row at a particular location in a table view
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path locating a row in tableView.
 * @return The movable property of the cell for the row indentified by indexPath.
 */
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    ListViewSectionWidget* section = [_children objectAtIndex:indexPath.section];
    if (!section)
    {
        return 0;
    }
    ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    return cellWidget.movable;
}

/**
 * Gets a new, restricted path of a proposed move for a row. By default, rows can only be moved
 * to places where the destination also is movable
 * @param tableView The table-view object requesting this information.
 * @param sourceIndexPath An index path locating a row in tableView the user started to move.
 * @param proposedDestinationIndexPath An index path locating a row in tableView the user is currently hovering
 * with the movable row.
 * @return A new index path where the row should end up when the user stops the move.
 *
 */
- (NSIndexPath *)tableView:(UITableView *)tableView targetIndexPathForMoveFromRowAtIndexPath:(NSIndexPath *)sourceIndexPath toProposedIndexPath:(NSIndexPath *)proposedDestinationIndexPath
{
	ListViewSectionWidget* section = [_children objectAtIndex:proposedDestinationIndexPath.section];
	if (!section)
		return sourceIndexPath;

	ListViewItemWidget* cellWidget = [section cellWidgetAtIndex:proposedDestinationIndexPath.row];
	if (!cellWidget.movable)
		return sourceIndexPath;

	return proposedDestinationIndexPath;
}

/**
 * Performs a row move.
 * @param tableView The table-view object requesting the move.
 * @param sourceIndexPath An index path locating a row in tableView the user started to move.
 * @param destinationIndexPath An index path locating a row in tableView the user wants to move the row.
 */
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)sourceIndexPath toIndexPath:(NSIndexPath *)destinationIndexPath
{
	if (sourceIndexPath.row == destinationIndexPath.row)
		return;

	ListViewSectionWidget* fromSection = [_children objectAtIndex:sourceIndexPath.section];
	if (!fromSection)
		return;

	ListViewSectionWidget* toSection = [_children objectAtIndex:destinationIndexPath.section];
	if (!toSection)
		return;

	NSNumber* insertIndex = [[NSNumber alloc] initWithInteger:destinationIndexPath.row];
	ListViewItemWidget* cellWidget = [fromSection cellWidgetAtIndex:sourceIndexPath.row];
	[fromSection removeChild:cellWidget];
	[toSection insertChild:cellWidget atIndex:insertIndex];
	[insertIndex release];

	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_LIST_ITEM_MOVED;
	eventData->sectionIndexBefore = sourceIndexPath.section;
	eventData->sectionIndexAfter = destinationIndexPath.section;
	eventData->sectionItemIndexBefore = sourceIndexPath.row;
	eventData->sectionItemIndexAfter = destinationIndexPath.row;
	eventData->widgetHandle = self.handle;
	event.data = (int)eventData;
	Base::gEventQueue.put(event);
}

- (BOOL)tableView:(UITableView *)tableView shouldIndentWhileEditingRowAtIndexPath:(NSIndexPath *)indexPath
{
    return _shouldIndentWhileEditing;
}

#pragma mark ListViewDelegate

/**
 * Delete an list view item index from the list.
 * @param indexPath List item index to be deleted.
 */
-(void)deleteItemAtIndexPath:(NSIndexPath*) indexPath
{
    if (!_isAnimating)
    {
        [self.tableView reloadData];
    }
    else
    {
        [self createAnimationType:ListViewItemAnimationTypeDelete itemPath:indexPath];
    }
}

/**
 * Insert an list view item into the list.
 * @param indexPath List item index where to be inserted.
 */
-(void)insertItemAtIndexPath:(NSIndexPath*) indexPath
{
    if (!_isAnimating)
    {
	    [self.tableView reloadData];
    }
    else
    {
        [self createAnimationType:ListViewItemAnimationTypeInsert itemPath:indexPath];
    }
}

/**
 * Reload an list view item. Its size has changed so it needs to be reloaded.
 * @param indexPath List item index that needs to be reloaded.
 */
-(void)reloadItemAtIndexPath:(NSIndexPath*) indexPath
{
    /*
     Note: reloadRowsAtIndexPaths does not resize item's height!
    [self.tableView reloadRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]
                          withRowAnimation:UITableViewRowAnimationNone];
     */
    if (!_isAnimating)
        [self.tableView reloadData];
}

/**
 * Reload the index for all sections.
 */
-(void)reloadListViewSectionIndexTitles
{
    /*
     * Note: reloadSectionIndexTitles is not working after the list is loaded.
     * [self.tableView reloadSectionIndexTitles];
     */
    if (!_isAnimating)
        [self.tableView reloadData];
}

/**
 * Reload an given section.
 * @param sectionIndex Section's index in list view.
 */
-(void)reloadListViewSectionAtIndex:(int)sectionIndex
{
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:sectionIndex]
                  withRowAnimation:UITableViewRowAnimationNone];
}

/**
 * Ask the table for the item's height.
 * @return The item height set in the table view.
 */
- (CGFloat)tableViewRowHeight
{
    return self.tableView.rowHeight;
}

-(BOOL)isAnimating
{
    return _isAnimating;
}

/**
 * Dealloc method.
 */
-(void) dealloc
{
    for (int i = 0; i < _itemOperations.count; ++i)
    {
        [[_itemOperations objectAtIndex:i] release];
    }
    [_itemOperations release];

    [super dealloc];
}
@end

/**
 * Private methods for ListViewWidget.
 */
@implementation ListViewWidget (hidden)

/**
 * Create a UITableView object and set the delegates.
 * If the list already has an view object it will be released and
 * a new one will be created.
 * The newest table view style will be set accordingly to the list view's type.
 * All its children will be realeased.
 */
-(void)createTableView
{
    [_children removeAllObjects];
    UITableView* tableView = (UITableView*)self.view;
    CGRect rect = CGRectZero;
    UIView* superview = nil;
    if (tableView)
    {
        rect = tableView.frame;
        tableView.delegate = nil;
        tableView.dataSource = nil;
        [tableView removeFromSuperview];
        superview = tableView.superview;
    }

    UITableViewStyle style = UITableViewStylePlain;
    if (_type == ListViewTypeSegmented)
    {
        style = UITableViewStyleGrouped;
    }
    tableView = [[UITableView alloc] initWithFrame:rect style:style];
    tableView.delegate = self;
    tableView.dataSource = self;
    self.view = tableView;

    if (superview)
    {
        [superview addSubview:tableView];
    }

    if (_type == ListViewTypeDefault)
    {
        ListViewSectionWidget* section = [[ListViewSectionWidget alloc] init];
        [self addChild:section toSubview:YES];
        section.index = 0;
        section.delegate = self;
    }
    [tableView release];
    tableView = nil;
}

/**
 * Set the list view type.
 * Setter for MAW_LIST_VIEW_TYPE.
 * All its children will be deleted.
 * Will create a new UITableView object.
 * @param listType Must be one of the following values:
 * - MAW_LIST_VIEW_TYPE_DEFAULT
 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
 * - MAW_LIST_VIEW_TYPE_SEGMENTED
 * @return An string object containing one of the following values:
 * - MAW_RES_OK if the list's type was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the listType param was invalid.
 */
-(int)setListViewTypeProperty:(NSString*)listType
{
    if (![listType canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int returnValue = MAW_RES_OK;
    ListViewType previousType = _type;
    switch ([listType intValue])
    {
        case MAW_LIST_VIEW_TYPE_DEFAULT:
            _type = ListViewTypeDefault;
            break;
        case MAW_LIST_VIEW_TYPE_ALPHABETICAL:
            _type = ListViewTypeAlphabetical;
            break;
        case MAW_LIST_VIEW_TYPE_SEGMENTED:
            _type = ListViewTypeSegmented;
            break;
        default:
            returnValue = MAW_RES_INVALID_PROPERTY_VALUE;
    }

    if (previousType != _type)
    {
        [self createTableView];
    }

    return returnValue;
}

/**
 * Get the list view type property.
 * Getter for MAW_LIST_VIEW_TYPE.
 * @return An NSString object containg the following valus:
 * - MAW_LIST_VIEW_TYPE_DEFAULT
 * - MAW_LIST_VIEW_TYPE_ALPHABETICAL
 * - MAW_LIST_VIEW_TYPE_SEGMENTED
 */
-(NSString*)getListViewTypeProperty
{
    int listType = MAW_LIST_VIEW_TYPE_DEFAULT;
    switch (_type)
    {
        case ListViewTypeAlphabetical:
            listType = MAW_LIST_VIEW_TYPE_ALPHABETICAL;
            break;
        case ListViewTypeSegmented:
            listType = MAW_LIST_VIEW_TYPE_SEGMENTED;
            break;
        case ListViewTypeDefault:
        default:
            listType = MAW_LIST_VIEW_TYPE_DEFAULT;
            break;
    }
    return [NSString stringWithFormat:@"%d", listType];
}

/**
 * Set the list view editing mode.
 * Setter for MAW_LIST_VIEW_MODE.
 * @param mode MAW_LIST_VIEW_MODE_DISPLAY will turn the editing mode to off, or
 * MAW_LIST_VIEW_MODE_EDIT will allow editing.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the mode param was invalid.
 */
-(int)setModeProperty:(NSString*) mode
{
    if (![mode canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    BOOL isModeParamValid = YES;
    BOOL editMode;
    switch ([mode intValue])
    {
        case MAW_LIST_VIEW_MODE_DISPLAY:
            editMode = NO;
            break;
        case MAW_LIST_VIEW_MODE_EDIT:
            editMode = YES;
            break;
        default:
            isModeParamValid = NO;
            break;
    }

    if (!isModeParamValid)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [self.tableView setEditing:editMode animated:YES];
    return MAW_RES_OK;
}

/**
 * Check if the list is in editing mode or not.
 * Getter for MAW_LIST_VIEW_MODE.
 * @return One of the following values:
 * - MAW_LIST_VIEW_MODE_DISPLAY if the list in not in the editing mdoe.
 * - MAW_LIST_VIEW_MODE_EDIT if the list is in the editing mode.
 */
-(NSString*) getModeProperty
{
    int modeInt = [self.tableView isEditing] ? MAW_LIST_VIEW_MODE_EDIT : MAW_LIST_VIEW_MODE_DISPLAY;
    return [NSString stringWithFormat:@"%d", modeInt];
}

-(int)setIndentWhileEditingProperty:(NSString*) shouldIndent
{
    BOOL shouldIndentBool;
    if ([shouldIndent isEqualToString:kWidgetTrueValue])
    {
        shouldIndentBool = YES;
    }
    else if ([shouldIndent isEqualToString:kWidgetFalseValue])
    {
        shouldIndentBool = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.shouldIndentWhileEditing = shouldIndentBool;
    return MAW_RES_OK;
}

-(NSString*) getIndentWhileEditingProperty
{
    return self.shouldIndentWhileEditing ? kWidgetTrueValue : kWidgetFalseValue;
}

/**
 * Enable/disable user interaction with an list view item.
 * A boolean value that determines whether the users can select a row will be set.
 * Setter for MAW_LIST_VIEW_ALLOW_SELECTION.
 * @param allowSelection "true" will allow the user to select an item, "false" will not
 * allow the user to select it.
 * @return One of the following values:
 * - MAW_RES_OK if the bool value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the allowSelection param was invalid.
 */
-(int)setAllowSelectionProperty:(NSString*) allowSelection
{
    BOOL allowSelectionBool;
    if ([allowSelection isEqualToString:kWidgetTrueValue])
    {
        allowSelectionBool = YES;
    }
    else if ([allowSelection isEqualToString:kWidgetFalseValue])
    {
        allowSelectionBool = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.tableView.allowsSelection = allowSelectionBool;
    return MAW_RES_OK;
}

/**
 * Check if user can select an list view item.
 * Getter for MAW_LIST_VIEW_ALLOW_SELECTION.
 * @return "true" if the user can select an item, "false" otherwise.
 */
-(NSString*) getAllowSelectionProperty
{
    return self.tableView.allowsSelection ? kWidgetTrueValue : kWidgetFalseValue;
}

-(int)setAllowSelectionDuringEditingProperty:(NSString*) allowSelection
{
    BOOL allowSelectionBool;
    if ([allowSelection isEqualToString:kWidgetTrueValue])
    {
        allowSelectionBool = YES;
    }
    else if ([allowSelection isEqualToString:kWidgetFalseValue])
    {
        allowSelectionBool = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.tableView.allowsSelectionDuringEditing = allowSelectionBool;
    return MAW_RES_OK;
}

-(NSString*) getAllowSelectionDuringEditingProperty
{
    return self.tableView.allowsSelectionDuringEditing ? kWidgetTrueValue : kWidgetFalseValue;
}

-(int)setAllowScrollingProperty:(NSString*) allowScrolling
{
    BOOL allowScrollingBool;
    if ([allowScrolling isEqualToString:kWidgetTrueValue])
    {
        allowScrollingBool = YES;
    }
    else if ([allowScrolling isEqualToString:kWidgetFalseValue])
    {
        allowScrollingBool = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    self.tableView.scrollEnabled = allowScrollingBool;
    return MAW_RES_OK;
}

-(NSString*) getAllowScrollingProperty
{
    return self.tableView.scrollEnabled ? kWidgetTrueValue : kWidgetFalseValue;
}

-(int)setBackgroundColor:(NSString*) colorValue
{
    UIColor* color = [UIColor colorWithHexString:colorValue];
    if(!color) return MAW_RES_INVALID_PROPERTY_VALUE;

    UIView* bgView = [[UIView alloc] init];
    bgView.backgroundColor = color;
    [self.tableView setBackgroundView:bgView];
    return MAW_RES_OK;
}

-(int) setAnimationState:(NSString*) value
{
    UITableView* tableView = (UITableView*) self.view;
    if ([value isEqualToString:kWidgetTrueValue])
    {
        _isAnimating = YES;
    }
    else
    {
        _isAnimating = NO;
        [tableView beginUpdates];
        NSMutableArray* indicesArray = [[NSMutableArray alloc] initWithCapacity:_itemOperations.count];

        // Do delete operations first, indicies from the original table layout
        for (int i = 0; i < _itemOperations.count; ++i)
        {
            ListViewItemAnimation* itemAnimation = (ListViewItemAnimation*)[_itemOperations objectAtIndex:i];
            if (itemAnimation.type == ListViewItemAnimationTypeDelete)
            {
                [indicesArray addObject:itemAnimation.indexPath];
            }
        }
        [tableView deleteRowsAtIndexPaths:indicesArray withRowAnimation:UITableViewRowAnimationFade];
        [indicesArray removeAllObjects];

        // Do insert operations after delete, indices from after the delete operation
        for (int i = 0; i < _itemOperations.count; ++i)
        {
            ListViewItemAnimation* itemAnimation = (ListViewItemAnimation*)[_itemOperations objectAtIndex:i];
            if (itemAnimation.type == ListViewItemAnimationTypeInsert)
            {
                [indicesArray addObject:itemAnimation.indexPath];
            }
        }
        [tableView insertRowsAtIndexPaths:indicesArray withRowAnimation:UITableViewRowAnimationFade];
        [tableView endUpdates];

        for (int i = 0; i < _itemOperations.count; ++i)
        {
            [[_itemOperations objectAtIndex:i] release];
        }
        [_itemOperations removeAllObjects];
        [indicesArray release];
    }

    return MAW_RES_OK;
}

-(void)createAnimationType:(ListViewItemAnimationType)type itemPath:(NSIndexPath*) indexPath
{
    ListViewItemAnimation* newAnimation = [[ListViewItemAnimation alloc] init];
    newAnimation.type = type;

    if (type == ListViewItemAnimationTypeDelete)
    {
        // Calculate row at animation start
        int rowAtAnimationStart = indexPath.row;
        for (int i = 0; i < _itemOperations.count; ++i)
        {
            ListViewItemAnimation* previousAnimation = (ListViewItemAnimation*)[_itemOperations objectAtIndex:i];
            if (indexPath.row > previousAnimation.indexPath.row)
            {
                if (previousAnimation.type == ListViewItemAnimationTypeInsert)
                {
                    --rowAtAnimationStart;
                }
                else if (previousAnimation.type == ListViewItemAnimationTypeDelete)
                {
                    ++rowAtAnimationStart;
                }
            }
        }

        newAnimation.indexPath = [NSIndexPath indexPathForRow:rowAtAnimationStart inSection:indexPath.section];

        // Move all insertions below the old row up by one
        for (int i = 0; i < _itemOperations.count; ++i)
        {
            ListViewItemAnimation* previousAnimation = (ListViewItemAnimation*)[_itemOperations objectAtIndex:i];
            if (newAnimation.indexPath.row < previousAnimation.indexPath.row)
            {
                if (previousAnimation.type == ListViewItemAnimationTypeInsert)
                {
                    previousAnimation.indexPath = [NSIndexPath indexPathForRow:previousAnimation.indexPath.row-1 inSection:previousAnimation.indexPath.section];
                }
            }
        }
    }
    else if (type == ListViewItemAnimationTypeInsert)
    {
        newAnimation.indexPath = indexPath;

        // Move all previous insertions below this row one row down
        for (int i = 0; i < _itemOperations.count; ++i)
        {
            ListViewItemAnimation* previousAnimation = (ListViewItemAnimation*)[_itemOperations objectAtIndex:i];
            if (previousAnimation.indexPath.row >= indexPath.row &&
                previousAnimation.type == ListViewItemAnimationTypeInsert)
            {
                previousAnimation.indexPath = [NSIndexPath indexPathForRow:previousAnimation.indexPath.row+1 inSection:previousAnimation.indexPath.section];
            }
        }
    }

    [_itemOperations addObject:newAnimation];
}

@end
