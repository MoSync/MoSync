/* Copyright (C) 2011 Mobile Sorcery AB

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
 * @file PimList.h
 * Abstract class for pim list.
 */

#import "PimList.h"

@implementation PimList

/**
 * Init function.
 */
-(id) init
{
	return [super init];
}

/**
 * Imports all pim items from database(e.g. Address Book).
 */
-(void) openList
{
	methodNotImplemented();
}

/**
 * Gets a handle to the next item in list.
 * @return A handle to the next item in list, or 0 if the are no more items,
 *		 or one of the MA_PIM_ERR constants in case of error.
 */
-(MAHandle) getNextItem
{
	methodNotImplemented();
}

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *		 an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle
{
	methodNotImplemented();
}

/**
 * Creates an pim item.
 * @return A handle to the new Contact item.
 */
-(MAHandle) createItem
{
	methodNotImplemented();
}

/**
 * Removes an pim item from the list.
 * @param itemHandle The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) itemHandle
{
	methodNotImplemented();
}

/**
 * Closes the list.
 * Adds the new items into database(e.g. Address Book).
 * Saves the modified items into database(e.g. Address Book).
 * Removes all items from the list.
 * @return One of MA_PIM_ERR contants.
 */
-(int) close
{
	methodNotImplemented();
}

/**
 * Closes a given item.
 * @param itemHandle A handle to a pim item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) closeItem:(MAHandle) itemHandle
{
    methodNotImplemented();
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
	[super dealloc];
}

@end
