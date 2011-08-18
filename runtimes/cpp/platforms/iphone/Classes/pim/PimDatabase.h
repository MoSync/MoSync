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

#import <Foundation/Foundation.h>
#import <AddressBook/AddressBook.h>

#include "helpers/cpp_defs.h"
#include "helpers/cpp_ix_pim.h"
#include "PimContactsList.h"
#import "PimUtil.h"
#import "PimItem.h"

@interface PimDatabase:  NSObject {
    /**
     * List with contact items.
     */
    PimList* mContactsList;
}

/**
 * Init function.
 */
-(id) init;

/**
 * Opens a pim list.
 * @param listType One of MA_PIM constants.
 * @return A pim list handle, or MA_PIM_ERR_UNAVAILABLE_LIST in case of error.
 */
-(MAHandle) pimListOpen:(const int) listType;

/**
 * Gets a handle to the next pim item in the specified list.
 * @param list The specified list.
 * @return A handle to the next item, or 0 if there are no more items,
 * or one of MA_PIM_ERR_ in case of error.
 */
-(MAHandle) pimListNext:(MAHandle) list;

/**
 * Closes the list.
 * @param list The specified list.
 * @return One of MA_PIM_ERR constants.
 */
-(int) pimListClose:(MAHandle) list;

/**
 * Closes a given pim list.
 * @param list The specified list.
 * @return One of MA_PIM_ERR constants.
 */
-(int) closeList:(PimList*) list;

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *         an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle;

-(MAHandle) createItem:(MAHandle) list;

/**
 * Removes an pim item from a specified list.
 * @param list The given list.
 * @param item The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) list
             item:(MAHandle) item;

@end
