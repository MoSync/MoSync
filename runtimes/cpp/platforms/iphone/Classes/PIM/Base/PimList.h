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
 * @file PimList.h
 * Abstract class for pim list.
 */

#import <Foundation/Foundation.h>
#import "PimUtil.h"
#import "PimItem.h"

/**
 * An emulated abstract class for pim list.
 * Objectiv C does not offer the possibility to create an abstract class
 * so this is an usual class, but each method(except init and dealloc)
 * raises an exception.
 * DO NOT USE THIS CLASS TO CREATE OBJECTS. Instead use classes that are
 * derived from this class(e.g. PimContactsList).
 * This class only defines the behavior for each pim list class.
 */
@interface PimList :  NSObject {

}

/**
 * Init function.
 */
-(id) init;

/**
 * Imports all pim items from database(e.g. Address Book).
 */
-(void) openList;

/**
 * Gets a handle to the next item in list.
 * @return A handle to the next item in list, or 0 if the are no more items,
 *         or one of the MA_PIM_ERR constants in case of error.
 */
-(MAHandle) getNextItem;

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *         an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle;

/**
 * Creates an pim item.
 * @return A handle to the new Contact item.
 */
-(MAHandle) createItem;

/**
 * Removes an pim item from the list.
 * @param itemHandle The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) itemHandle;

/**
 * Closes the list.
 * Adds the new items into database(e.g. Address Book).
 * Saves the modified items into database(e.g. Address Book).
 * Removes all items from the list.
 * @return One of MA_PIM_ERR contants.
 */
-(int) close;

/**
 * Closes a given item.
 * @param itemHandle A handle to a pim item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) closeItem:(MAHandle) itemHandle;

@end
