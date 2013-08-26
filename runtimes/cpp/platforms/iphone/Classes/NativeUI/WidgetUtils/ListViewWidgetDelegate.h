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
 * @file ListViewWidgetDelegate.h
 * @author Bogdan Iusco
 *
 * @brief ListView widget delegate.
 * Used by ListViewSectionWidget objects to notify the list view that its
 * content has been changed.
 */

#import <Foundation/Foundation.h>

@protocol ListViewWidgetDelegate <NSObject>

@required
/**
 * Delete an list view item from the list.
 * @param indexPath List item index to be deleted.
 */
-(void)deleteItemAtIndexPath:(NSIndexPath*) indexPath;

/**
 * Insert an list view item into the list.
 * @param indexPath List item index where to be inserted.
 */
-(void)insertItemAtIndexPath:(NSIndexPath*) indexPath;

/**
 * Reload an list view item. Its size has changed so it needs to be reloaded.
 * @param indexPath List item index that needs to be reloaded.
 */
-(void)reloadItemAtIndexPath:(NSIndexPath*) indexPath;

/**
 * Reload the index for all sections.
 */
-(void)reloadListViewSectionIndexTitles;

/**
 * Reload an given section.
 * @param sectionIndex Section's index in list view.
 */
-(void)reloadListViewSectionAtIndex:(int)sectionIndex;

/**
 * Ask the table for the item's height.
 * @return The item height set in the table view.
 */
- (CGFloat)tableViewRowHeight;

@end
