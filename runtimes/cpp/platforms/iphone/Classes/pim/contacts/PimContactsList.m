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

#import "PimContactsList.h"
#import "PimUtil.h"

@implementation PimContactsList

/**
 * Init function.
 */
-(id) init
{
    mContactsDictionary = [[NSMutableDictionary alloc] init];
    mKeysArrayIndex = MA_PIM_ERR_HANDLE_INVALID;
    return [super init];
}

/**
 * Imports all contact items from Address Book.
 */
-(void) openList
{
    // Copy all contacts.
    mAddressBook = ABAddressBookCreate();
    CFMutableArrayRef contactsArray;
    CFArrayRef people = ABAddressBookCopyArrayOfAllPeople(mAddressBook);
    contactsArray = CFArrayCreateMutableCopy(
        kCFAllocatorDefault,
        CFArrayGetCount(people),
        people);

    // Sort the contacts by name.
    int countContacts = CFArrayGetCount(contactsArray);
    CFArraySortValues(
        contactsArray,
        CFRangeMake(0, countContacts),
        (CFComparatorFunction) ABPersonComparePeopleByName,
        (void*) ABPersonGetSortOrdering());

    // Save the contact items into a dictionary. In the dictionary each contact item
    // will have associated a key(the item's handle). Each key is unique and it's
    // generated using getNextHandle function(PimUtils class).
    for(int i = 0; i < countContacts; i++)
    {
        // Get the contact item from the array.
        ABRecordRef nativeContact = CFArrayGetValueAtIndex(contactsArray, i);
        PimContactItem* item = [[PimContactItem alloc] initWithRecord:nativeContact];

        // Get the next handle.
        int handle = [[PimUtils sharedInstance] getNextHandle];
        NSString* key = [[NSString alloc] initWithFormat:@"%d", handle];

        // Save the item into dictionary.
        [mContactsDictionary setObject:item forKey: key];
        NSLog(@"contact item added-handle = %@", key);
        [key release];
    }

    CFRelease(contactsArray);
    CFRelease(people);
    mKeysArrayIndex = -1;
}

/**
 * Gets a handle to the next item in list.
 * @return A handle to the next item in list, or 0 if the are no more items,
 *         or one of the MA_PIM_ERR constants in case of error.
 */
-(MAHandle) getNextItem
{
    // Check if the list is opened.
    if(MA_PIM_ERR_HANDLE_INVALID == mKeysArrayIndex)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    // Check if the are more items in list.
    NSArray* keysArray = [mContactsDictionary allKeys];
    if (mKeysArrayIndex == ([keysArray count] -1))
    {
        return 0;
    }

    // Sort the values in the array so the last values will last added.
    // The created contacts will be at the end of the array.
    NSArray* sortedKeysArray = [keysArray sortedArrayUsingComparator:
        ^(id obj1, id obj2)
     {
         if ([obj1 intValue] > [obj2 intValue])
         {
             return (NSComparisonResult) NSOrderedDescending;
         }
         if ([obj1 integerValue] < [obj2 integerValue]) {
             return (NSComparisonResult)NSOrderedAscending;
         }
         return (NSComparisonResult)NSOrderedSame;
     }];

    // Return a handle to the next item.
    mKeysArrayIndex++;
    NSString* currentStringKey = [sortedKeysArray objectAtIndex:mKeysArrayIndex];
    int currentIntKey = [currentStringKey intValue];

    return currentIntKey;
}

/**
 * Gets an pim item for a given handle.
 * @param itemHandle The specified handle.
 * @return The item, or nil if the database does not contain
 *         an item with the given handle.
 */
-(PimItem*) getItem:(MAHandle) itemHandle
{
    NSString* key = [[NSString alloc] initWithFormat:@"%d",itemHandle];
    PimItem* contactItem = [mContactsDictionary objectForKey:key];
    [key release];

    return contactItem;
}

/**
 * Creates an Contact item.
 * @return A handle to the new Contact item.
 */
-(MAHandle) createItem
{
    PimContactItem* item = [[PimContactItem alloc] init];
    int handle = [[PimUtils sharedInstance] getNextHandle];
    NSString* key = [[NSString alloc] initWithFormat:@"%d", handle];
    NSLog(@"createItem PimContactList --- create item with key = %@", key);

    [mContactsDictionary setObject:item forKey: key];
    [key release];

    return handle;
}

/**
 * Closes the list.
 * Adds the new items into Address Book.
 * Saves the modified items into Address Book.
 * Removes all items from the list.
 * @return One of MA_PIM_ERR contants.
 */
-(int) close
{
    int returnedValue = MA_PIM_ERR_NONE;

    NSArray* keysArray = [mContactsDictionary allKeys];
    int countContacts = [keysArray count];
    for (int i = 0; i < countContacts; i++)
    {
        NSString* key = [keysArray objectAtIndex:i];
        PimContactItem* item = [mContactsDictionary objectForKey:key];
        returnedValue = [self saveItemInAddressBook:item];
        if (MA_PIM_ERR_NONE != returnedValue)
        {
            break;
        }
    }

    [keysArray release];

    bool isSaved = ABAddressBookSave(mAddressBook, NULL);

    // Check if the Address Book was saved.
    if (!isSaved)
    {
        returnedValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
    }

    // If no error occurred remove all the items from dictionary.
    if (MA_PIM_ERR_NONE == returnedValue)
    {
        [mContactsDictionary removeAllObjects];
    }

    return returnedValue;
}

/**
 * Closes a given item.
 * @param itemHandle A handle to a pim item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) closeItem:(MAHandle) itemHandle
{
    PimContactItem* item = (PimContactItem*)[self getItem:itemHandle];
    if(!item)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    int resultCode = MA_PIM_ERR_NONE;
    resultCode = [self saveItemInAddressBook:item];

    bool isSaved = ABAddressBookSave(mAddressBook, NULL);
    // Check if the Address Book was saved.
    if (!isSaved)
    {
       resultCode = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
   }

    return resultCode;
}

/**
 * Adds an item in Address Book.
 * @param item The given item.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) saveItemInAddressBook:(PimContactItem*) item
{
    PimItemStatus status = [item getStatus];
    int returnedValue = MA_PIM_ERR_NONE;

    // Write item's field into record.
    [item close];

    if (kNewItem == status)
    {
        bool isAdded = ABAddressBookAddRecord(mAddressBook, [item getRecord], NULL);

        // Check if the item was added to Address Book.
        if (!isAdded)
        {
            returnedValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
        }
    }

    return returnedValue;
}

/**
 * Removes an pim item from this list.
 * @param itemHandle The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) itemHandle
{
    PimContactItem* item = (PimContactItem*)[self getItem:itemHandle];
    if (!item)
    {
        return MA_PIM_ERR_HANDLE_INVALID;
    }

    PimItemStatus status = [item getStatus];
    NSString* key = [[NSString alloc] initWithFormat:@"%d",itemHandle];
    ABAddressBookRemoveRecord(mAddressBook, [item getRecord], nil);

    [mContactsDictionary removeObjectForKey:key];
    [item release];
    [key release];
    if (kNewItem != status)
    {
        mKeysArrayIndex--;
    }

    return MA_PIM_ERR_NONE;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    CFRelease(mAddressBook);
    [mContactsDictionary release];

    [super dealloc];
}

@end
