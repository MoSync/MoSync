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

#import "PimDatabase.h"
#import "MoSyncPanic.h"
#import "PimError.h"

@implementation PimDatabase

/**
 * Init function.
 */
-(id) init
{
	return [super init];
}

/**
 * Opens a pim list.
 * @param listType One of MA_PIM constants.
 * @return A pim list handle, or one of MA_PIM_ERR constants in case of error
 */
-(MAHandle) pimListOpen:(const int) listType
{
    if (MA_PIM_CONTACTS == listType)
	{
        if (!mContactsList)
        {
            mContactsList = [[PimContactsList alloc] init];
            [mContactsList openList];
            return MA_PIM_CONTACTS;
        }
        else
        {
            return MA_PIM_ERR_LIST_ALREADY_OPENED;
        }
	}
	else if(MA_PIM_EVENTS == listType ||
            MA_PIM_TODOS == listType)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_LIST_UNAVAILABLE
                                  withPanicCode:PANIC_LIST_UNAVAILABLE
                                  withPanicText:@PANIC_LIST_UNAVAILABLE_TEXT];
    }
    else
	{
		return [[MoSyncPanic getInstance] error:MA_PIM_ERR_LIST_TYPE_INVALID
                                  withPanicCode:PANIC_LIST_TYPE_INVALID
                                  withPanicText:@PANIC_LIST_TYPE_INVALID_TEXT];
	}
}

/**
 * Gets a handle to the next pim item in the specified list.
 * @param list The specified list.
 * @return A handle to the next item, or 0 if there are no more items,
 * or one of MA_PIM_ERR in case of error.
 */
-(MAHandle) pimListNext:(MAHandle) list
{
	MAHandle returnedValue = MA_PIM_ERR_HANDLE_INVALID;

	if (MA_PIM_CONTACTS == list)
	{
		if (mContactsList)
		{
            returnedValue = [mContactsList getNextItem];
		}
	}

    return returnedValue;
}

/**
 * Closes the list.
 * @param list The specified list.
 * @return One of MA_PIM_ERR constants.
 */
-(int) pimListClose:(MAHandle) list
{
	int returnedValue;

	if (MA_PIM_CONTACTS == list)
	{
        returnedValue = [self closeList:mContactsList];
		if (MA_PIM_ERR_NONE == returnedValue)
		{
			mContactsList = nil;
		}
	}
	else
	{
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
	}

	return returnedValue;
}

/**
 * Closes a given pim list.
 * @param list The specified list.
 * @return One of MA_PIM_ERR constants.
 */
-(int) closeList:(PimList*) list;
{
	int returnedValue;

	if (!list)
	{
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
	}
	else
	{
		returnedValue = [list close];

		if (MA_PIM_ERR_NONE == returnedValue)
		{
			[list release];
		}
	}

	return returnedValue;
}

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *		 an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle
{
	PimItem* item = nil;
    if (mContactsList)
    {
        return [mContactsList getItem:itemHandle];
    }

	return item;
}

/**
 * Creates a new pim item for a given pim list.
 * @param list The given pim list.
 * @return A handle to a new pim item.
 */
-(MAHandle) createItem:(MAHandle) list
{
	if (list == MA_PIM_CONTACTS)
	{
        if (mContactsList)
        {
            return [mContactsList createItem];
        }
	}

    return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                              withPanicCode:PANIC_HANDLE_INVALID
                              withPanicText:@PANIC_HANDLE_INVALID_TEXT];
}

/**
 * Closes a given item.
 * @param itemHandle A handle to a pim item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) closeItem:(MAHandle) itemHandle
{
    if (mContactsList)
    {
        return [mContactsList closeItem:itemHandle];
    }

    return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                              withPanicCode:PANIC_HANDLE_INVALID
                              withPanicText:@PANIC_HANDLE_INVALID_TEXT];
}

/**
 * Removes an pim item from a specified list.
 * @param list The given list.
 * @param item The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) list
	item:(MAHandle) item
{
	if (list == MA_PIM_CONTACTS)
	{
        if (mContactsList)
        {
            return [mContactsList removeItem:item];
        }
	}

    return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                              withPanicCode:PANIC_HANDLE_INVALID
                              withPanicText:@PANIC_HANDLE_INVALID_TEXT];
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
	[mContactsList release];
	[super dealloc];
}

@end
