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
 * @return A pim list handle, or MA_PIM_ERR_UNAVAILABLE_LIST in case of error.
 */
-(MAHandle) pimListOpen:(const int) listType
{
    MAHandle returnedValue;

    if (MA_PIM_CONTACTS == listType)
    {
        mContactsList = [[PimContactsList alloc] init];
        [mContactsList openList];
        returnedValue = MA_PIM_CONTACTS;
    }
    else
    {
        returnedValue = MA_PIM_ERR_UNAVAILABLE_LIST;
    }
    
    return returnedValue;
}

/**
 * Gets a handle to the next pim item in the specified list.
 * @param list The specified list.
 * @return A handle to the next item, or 0 if there are no more items,
 * or one of MA_PIM_ERR_ in case of error.
 */
-(MAHandle) pimListNext:(MAHandle) list
{
    MAHandle returnedValue;

    if (MA_PIM_CONTACTS == list)
    {
        if (nil == mContactsList)
        {
            returnedValue = MA_PIM_ERR_LIST_NOT_OPENED;
        }
        else
        {
            returnedValue = [mContactsList getNextItem];   
        }
    }
    else
    {
        returnedValue = MA_PIM_ERR_UNAVAILABLE_LIST;
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
    MAHandle returnedValue;
    
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
        returnedValue = MA_PIM_ERR_UNAVAILABLE_LIST;
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
    
    if (nil == list)
    {
        returnedValue = MA_PIM_ERR_LIST_NOT_OPENED;
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
 *         an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle
{    
    PimItem* item = [mContactsList getItem:itemHandle];
       
    return item;
}

-(MAHandle) createItem:(MAHandle) list
{
    if (list == MA_PIM_CONTACTS) 
    {
        return [mContactsList createItem];   
    }

    return MA_PIM_ERR_UNAVAILABLE_LIST;
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
        return [mContactsList removeItem:item];    
    }
    
    return MA_PIM_ERR_UNAVAILABLE_LIST;
}

/**
 * Release all the objects.
 */
- (void) dealloc {
    
    [mContactsList release];
    [super dealloc];
}

@end
