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
    mKeysArrayIndex = MA_PIM_ERR_LIST_NOT_OPENED;
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
    if(MA_PIM_ERR_LIST_NOT_OPENED == mKeysArrayIndex)
    {
        return MA_PIM_ERR_LIST_NOT_OPENED;
    }

    // Check if the are more items in list.
    NSArray* keysArray = [mContactsDictionary allKeys];
    if ((mKeysArrayIndex + 1) >= [keysArray count])
    {
        return 0;
    }

    // Return a handle to the next item.
    mKeysArrayIndex++;
    NSString* currentStringKey = [keysArray objectAtIndex:mKeysArrayIndex];
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
-(MAHandle) createItem;
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
    CFErrorRef error = NULL;

    NSArray* keysArray = [mContactsDictionary allKeys];
    int countContacts = [keysArray count];
    for (int i = 0; i < countContacts; i++)
    {
        NSString* key = [keysArray objectAtIndex:i];
        PimContactItem* item = [mContactsDictionary objectForKey:key];
        PimItemStatus status = [item getStatus];

        if (kNewItem == status)
        {
            // Write item's field into record.
            [item close];

            bool isAdded = ABAddressBookAddRecord(mAddressBook, [item getRecord], &error);

            // Check if the item was added to Address Book.
            if (!isAdded)
            {
                returnedValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
                break;
            }
        }
        else if (kModifiedItem == status)
        {
            // Remove the old record from Address Book.
            bool isRemoved = ABAddressBookRemoveRecord(mAddressBook, [item getRecord], &error);
            // Check if the item was removed from Address Book.
            if (!isRemoved)
            {
                returnedValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
                break;
            }

            // Write item's field into record.
            [item close];

            // Add the new item to Address Book.
            bool isAdded = ABAddressBookAddRecord(mAddressBook, [item getRecord], &error);

            // Check if the item was added to Address Book.
            if (!isAdded)
            {
                returnedValue = MA_PIM_ERR_OPERATION_NOT_PERMITTED;
                break;
            }
        }
    }

    [keysArray release];

    bool isSaved = ABAddressBookSave(mAddressBook, &error);

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
 * Removes an pim item from this list.
 * @param item The given item.
 * @return One of the MA_PIM_ERR constants.
 **/
-(int) removeItem:(MAHandle) item
{
    NSString* key = [[NSString alloc] initWithFormat:@"%d",item];
    [mContactsDictionary removeObjectForKey:key];
    [key release];

    return MA_PIM_ERR_NONE;
}

/**
 * Release all the objects.
 */
- (void) dealloc {

    CFRelease(mAddressBook);
    [mContactsDictionary release];

    [super dealloc];
}

@end
