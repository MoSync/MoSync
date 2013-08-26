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
 *         or one of the MA_PIM_ERR constants in case of error.
 */
-(MAHandle) getNextItem
{
    methodNotImplemented();
}

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *         an item with the given handle.
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
