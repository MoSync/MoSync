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
 * @file SegmentedListViewWidget.mm
 * @author Bogdan Iusco
 * @date 22 Feb 2012
 *
 * @brief SegmentedListView widget implementation file.
 * A SegmentedListView widget is used to present an indexed/grouped list of items.
 */

/**
 * Default title for the delete-confirmation button, which appears on the right side of the cell.
 */
#define DEFAULT_DELETE_BUTTON_TITLE "Delete";

#import "SegmentedListViewWidget.h"
#import "SegmentedListViewSectionWidget.h"
#import "SegmentedListViewItemWidget.h"
#import "NSStringExpanded.h"

/**
 * Private methods for SegmentedListViewWidget.
 */
@interface SegmentedListViewWidget(hidden)

/**
 * Set the list type.
 * @param listType Must be one of the next constants:
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROEPRTY_VALUE if listType is not valid.
 */
-(int) setListType:(NSString*) listType;

/**
 * Get the list type.
 * @return One of the next values:
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED
 */
-(NSString*) listType;

/**
 * Set the list mode.
 * @param listMode Must be one of the next constants:
 * - MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY
 * - MAW_SEGMENTED_LIST_VIEW_MODE_EDIT
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROPERTY_VALUE if listMode is not valid.
 */
-(int) setListMode:(NSString*) listMode;

/**
 * Get the list mode.
 * @return One of the next values:
 * - MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY
 * - MAW_SEGMENTED_LIST_VIEW_MODE_EDIT
 */
-(NSString*) listMode;

/**
 * Set the allow selection flag.
 * @param listMode "true" or "false".
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROPERTY_VALUE if allowSelection is not valid.
 */
-(int) setAllowSelection:(NSString*) allowSelection;

/**
 * Get the allow selection value.
 * @return "true" or "false".
 */
-(NSString*) allowSelection;

@end

@implementation SegmentedListViewWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        _tableView = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
        _tableView.delegate = self;
        _tableView.dataSource = self;
        view = _tableView;

        _sections = [[NSMutableArray alloc] init];
    }

    return self;
}

/**
 * Sets a property.
 * @param key The property of the widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    int resultCode = MAW_RES_OK;
    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_TYPE])
    {
        resultCode = [self setListType:value];
    }
    else if([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_MODE])
    {
        resultCode = [self setListMode:value];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_RELOAD_DATA])
    {
        [_tableView reloadData];
    }
    else if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ALLOW_SELECTION])
    {
        resultCode = [self setAllowSelection:value];
    }
    else
    {
        resultCode = [super setPropertyWithKey:key toValue:value];
    }
    return resultCode;
}

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    if ([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_TYPE])
    {
        return [[self listType] retain];
    }
    else if([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_MODE])
    {
        return [[self listMode] retain];
    }
    else if([key isEqualToString:@MAW_SEGMENTED_LIST_VIEW_ALLOW_SELECTION])
    {
        return [[self allowSelection] retain];
    }

    return [super getPropertyWithKey:key];
}

/**
 * Add a section to list.
 */
- (void)addChild: (SegmentedListViewSectionWidget*) section
{
    LOGIN;
	[_sections addObject:section];
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
    SegmentedListViewSectionWidget* tableSection = [_sections objectAtIndex:section];
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
    UITableViewCell *cell = [_tableView dequeueReusableCellWithIdentifier:@"Cell"];
    if (cell == nil)
    {
        SegmentedListViewSectionWidget* section = [_sections objectAtIndex:indexPath.section];
        if (!section)
        {
            return nil;
        }

        SegmentedListViewItemWidget *cellWidget = [section cellWidgetAtIndex:indexPath.row];
        if (!cellWidget)
        {
            return nil;
        }
        cell = cellWidget.cell;
    }
    return cell;
}

/**
 * Return the number of sections in the table view.
 * @param tableView The table-view object requesting this information.
 * @return the number of sections in the table view.
 */
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [_sections count];
}

/**
 * Return the title of the header of the specified section of the table view.
 * @param tableView The table-view object requesting this information.
 * @param section An index number identifying a section of tableView.
 * @return A string to use as the title of the section header.
 */
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    SegmentedListViewSectionWidget* tableViewSection = [_sections objectAtIndex:section];
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
    SegmentedListViewSectionWidget* tableViewSection = [_sections objectAtIndex:section];
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
    return YES;
}

/**
 * Verify if a given row can be moved to another location in the table view.
 * @param tableView The table-view object requesting this information.
 * @param indexPath An index path locating a row in tableView.
 * @return YES if the row can be moved; otherwise NO.
 */
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
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
    for (SegmentedListViewSectionWidget* section in _sections)
    {
        if (section.title)
        {
            [array addObject:section.title];
        }
    }

    return array;
}

/**
 * Move a row at a specific location in the table view to another location.
 * @param tableView The table-view object requesting this action.
 * @param fromIndexPath An index path locating the row to be moved in tableView.
 * @param toIndexPath An index path locating the row in tableView that is the destination of the move.
 */
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
    NSLog(@"IN %s", __FUNCTION__);
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
    SegmentedListViewSectionWidget* section = [_sections objectAtIndex:indexPath.section];
    if (!section)
    {
        return 0;
    }
    SegmentedListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
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
    LOGIN;
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SEGMENTED_ITEM_WILL_SELECT;
	eventData->widgetHandle = handle;
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
    LOGIN;
	MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SEGMENTED_ITEM_SELECTED;
	eventData->widgetHandle = handle;
	eventData->listItemIndex = [indexPath row];
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
    SegmentedListViewSectionWidget* section = [_sections objectAtIndex:indexPath.section];
    if (!section)
    {
        return @DEFAULT_DELETE_BUTTON_TITLE;
    }
    SegmentedListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
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
    SegmentedListViewSectionWidget* section = [_sections objectAtIndex:indexPath.section];
    if (!section)
    {
        return UITableViewCellEditingStyleNone;
    }
    SegmentedListViewItemWidget* cellWidget = [section cellWidgetAtIndex:indexPath.row];
    if (!cellWidget)
    {
        return UITableViewCellEditingStyleNone;
    }
    return cellWidget.editingStyle;
}

/**
 * Release all member data.
 */
-(void) dealloc
{
    // Do NOT release _tableView object here. It will be released by the super class.
    [_sections release];

    [super dealloc];
}

@end

@implementation SegmentedListViewWidget(hidden)


/**
 * Set the list type.
 * @param listType Must be one of the next constants:
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROEPRTY_VALUE if listType is not valid.
 */
-(int) setListType:(NSString*) listType
{
    if (![listType  canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int listTypeValue = [listType intValue];
    UITableViewStyle listStyle;
    if (listTypeValue == MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN)
    {
        listStyle = UITableViewStylePlain;
    }
    else if (listTypeValue == MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED)
    {
        listStyle = UITableViewStyleGrouped;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    CGRect rect = _tableView.bounds;
    [_tableView release];
    _tableView = [[UITableView alloc] initWithFrame:rect style:listStyle];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    view = _tableView;

    return MAW_RES_OK;
}

/**
 * Get the list type.
 * @return One of the next values:
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN
 * - MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED
 */
-(NSString*) listType
{
    UITableViewStyle listStyle = _tableView.style;
    int listType;
    if (listStyle == UITableViewStylePlain)
    {
        listType = MAW_SEGMENTED_LIST_VIEW_TYPE_PLAIN;
    }
    else if(listStyle == UITableViewStyleGrouped)
    {
        listType = MAW_SEGMENTED_LIST_VIEW_TYPE_GROUPED;
    }

    return [NSString stringWithFormat:@"%d", listType];
}

/**
 * Set the list mode.
 * @param listMode Must be one of the next constants:
 * - MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY
 * - MAW_SEGMENTED_LIST_VIEW_MODE_EDIT
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROEPRTY_VALUE if listMode is not valid.
 */
-(int) setListMode:(NSString*) listMode
{
    if (![listMode  canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    int listModeValue = [listMode intValue];
    BOOL editMode;
    if (listModeValue == MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY)
    {
        editMode = NO;
    }
    else if (listModeValue == MAW_SEGMENTED_LIST_VIEW_MODE_EDIT)
    {
        editMode = YES;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    [_tableView setEditing:editMode animated:YES];
    return MAW_RES_OK;
}

/**
 * Get the list mode.
 * @return One of the next values:
 * - MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY
 * - MAW_SEGMENTED_LIST_VIEW_MODE_EDIT
 */
-(NSString*) listMode
{
    BOOL editMode = _tableView.editing;
    int listMode;
    if (editMode)
    {
        listMode = MAW_SEGMENTED_LIST_VIEW_MODE_EDIT;
    }
    else
    {
        listMode = MAW_SEGMENTED_LIST_VIEW_MODE_DISPLAY;
    }

    return [NSString stringWithFormat:@"%d", listMode];
}

/**
 * Set the allow selection flag.
 * @param listMode "true" or "false".
 * @return One of the next constants:
 * - MAW_RES_OK if no error occured.
 * - MAW_RES_INVALID_PROPERTY_VALUE if allowSelection is not valid.
 */
-(int) setAllowSelection:(NSString*) allowSelection
{
    BOOL allowSelectionFlag;
    if ([allowSelection isEqualToString:@"true"])
    {
        allowSelectionFlag = YES;
    }
    else if ([allowSelection isEqualToString:@"false"])
    {
        allowSelectionFlag = NO;
    }
    else
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    _tableView.allowsSelection = allowSelectionFlag;
    return MAW_RES_OK;
}

/**
 * Get the allow selection value.
 * @return "true" or "false".
 */
-(NSString*) allowSelection
{
    return _tableView.allowsSelection ? @"true" : @"false";
}

@end