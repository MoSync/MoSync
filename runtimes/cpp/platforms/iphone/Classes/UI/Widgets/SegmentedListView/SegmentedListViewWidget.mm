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

#import "SegmentedListViewWidget.h"
#import "SegmentedListViewSection.h"
#import "SegmentedListViewItemWidget.h"

@implementation SegmentedListViewWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UITableView* tableView = [[UITableView alloc] init];
        tableView.delegate = self;
        tableView.dataSource = self;
        view = tableView;

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
    return [super setPropertyWithKey:key toValue:value];
}

/**
 * Returns a property value of the widget.
 * @param key The property of the widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    return [super getPropertyWithKey:key];
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
    SegmentedListViewSection* tableSection = [_sections objectAtIndex:section];
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
    SegmentedListViewSection* section = [_sections objectAtIndex:indexPath.section];
    if (!section)
    {
        return nil;
    }

    SegmentedListViewItemWidget *cellWidget = [section getCellWidget:indexPath.row];
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
    SegmentedListViewSection* tableViewSection = [_sections objectAtIndex:section];
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
    SegmentedListViewSection* tableViewSection = [_sections objectAtIndex:section];
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
    for (SegmentedListViewSection* section in _sections)
    {
        [array addObject:section.title];
    }

    return array;
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
    SegmentedListViewSection* section = [_sections objectAtIndex:indexPath.section];
    if (!section)
    {
        return 0;
    }
    SegmentedListViewItemWidget* cellWidget = [section getCellWidget:indexPath.row];
    return cellWidget.height;
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
