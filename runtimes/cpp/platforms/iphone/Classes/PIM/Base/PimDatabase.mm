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
