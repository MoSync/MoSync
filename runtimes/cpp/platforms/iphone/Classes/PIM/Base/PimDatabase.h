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
 * @return A pim list handle, or one of MA_PIM_ERR constants in case of error
 */
-(MAHandle) pimListOpen:(const int) listType;

/**
 * Gets a handle to the next pim item in the specified list.
 * @param list The specified list.
 * @return A handle to the next item, or 0 if there are no more items,
 * or one of MA_PIM_ERR in case of error.
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

/**
 * Creates a new pim item for a given pim list.
 * @param list The given pim list.
 * @return A handle to a new pim item.
 */
-(MAHandle) createItem:(MAHandle) list;

/**
 * Closes a given item.
 * @param itemHandle A handle to a pim item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) closeItem:(MAHandle) itemHandle;

/**
 * Removes an pim item from a specified list.
 * @param list The given list.
 * @param item The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) list
             item:(MAHandle) item;

@end
