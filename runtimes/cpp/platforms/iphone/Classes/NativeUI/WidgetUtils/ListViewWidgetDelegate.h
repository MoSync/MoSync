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
