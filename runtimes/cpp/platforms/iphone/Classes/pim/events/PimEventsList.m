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

#import "PimEventsList.h"
#include "PimUtil.h"

@implementation PimEventsList

/**
 * Init function.
 */
-(id) init
{
    mEventsDictionary = [[NSMutableDictionary alloc] init];
    mEventStore = [[EKEventStore alloc] init];
    mKeysArrayIndex = MA_PIM_ERR_LIST_NOT_OPENED;
    
    return [super init];
}

/**
 * Imports all event items from Calendar.
 */
-(void) openList
{
    // Create the predicate's start and end dates.
    CFGregorianDate gregorianStartDate, gregorianEndDate;
    CFGregorianUnits startUnits = {-2, 0, 0, 0, 0, 0};
    CFGregorianUnits endUnits = {2, 0, 0, 0, 0, 0};
    CFTimeZoneRef timeZone = CFTimeZoneCopySystem();
    
    gregorianStartDate = CFAbsoluteTimeGetGregorianDate(
        CFAbsoluteTimeAddGregorianUnits(CFAbsoluteTimeGetCurrent(), timeZone, startUnits),
        timeZone);
    gregorianStartDate.hour = 0;
    gregorianStartDate.minute = 0;
    gregorianStartDate.second = 0;
    
    gregorianEndDate = CFAbsoluteTimeGetGregorianDate(
        CFAbsoluteTimeAddGregorianUnits(CFAbsoluteTimeGetCurrent(), timeZone, endUnits),
        timeZone);
    gregorianEndDate.hour = 0;
    gregorianEndDate.minute = 0;
    gregorianEndDate.second = 0;
    
    NSDate* startDate =
    [NSDate dateWithTimeIntervalSinceReferenceDate:CFGregorianDateGetAbsoluteTime(gregorianStartDate, timeZone)];
    NSDate* endDate =
    [NSDate dateWithTimeIntervalSinceReferenceDate:CFGregorianDateGetAbsoluteTime(gregorianEndDate, timeZone)];
    CFRelease(timeZone);
    
    // Create the predicate.
    NSArray* array = [NSArray arrayWithObject:[mEventStore defaultCalendarForNewEvents]];
    NSPredicate *predicate = [mEventStore predicateForEventsWithStartDate:startDate 
                                                                  endDate:endDate 
                                                                calendars:array]; 
    
    // Fetch all events that match the predicate.
    NSArray *events = [mEventStore eventsMatchingPredicate:predicate];
    for (EKEvent* event in events)
    {
        PimItem* pimEvent = [[PimEventItem alloc] initWithEvent:event];
       
        // Get the next handle.
        int handle = [[PimUtils sharedInstance] getNextHandle];
        NSString* key = [[NSString alloc] initWithFormat:@"%d", handle];
        
        // Save the item into dictionary.
        [mEventsDictionary setObject:pimEvent forKey: key];
        [key release];
    }
    
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
    NSArray* keysArray = [mEventsDictionary allKeys];
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
    PimItem* contactItem = [mEventsDictionary objectForKey:key];
    [key release];
    
    return contactItem;
}

/**
 * Creates an Event item.
 * @return A handle to the new Event item.
 */
-(MAHandle) createItem;
{
    PimEventItem* item = [[PimEventItem alloc] init];
    int handle = [[PimUtils sharedInstance] getNextHandle];
    NSString* key = [[NSString alloc] initWithFormat:@"%d", handle];
    NSLog(@"createItem PimEventsList --- create item with key = %@", key);
    
    [mEventsDictionary setObject:item forKey: key];
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
    
    NSArray* keysArray = [mEventsDictionary allKeys];
    int countContacts = [keysArray count];
    for (int i = 0; i < countContacts; i++) 
    {
        NSString* key = [keysArray objectAtIndex:i];
        PimEventItem* item = [mEventsDictionary objectForKey:key];
        PimItemStatus status = [item getStatus];
        
        if (kNewItem == status)
        {
            // Write item's field into record.
            [item close];
            
        }
        else if (kModifiedItem == status)
        {
        }
    }
    
    [keysArray release];
    
    // If no error occurred remove all the items from dictionary.
    if (MA_PIM_ERR_NONE == returnedValue) 
    {
        [mEventsDictionary removeAllObjects];
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
    [mEventsDictionary removeObjectForKey:key];
    [key release];
    
    return MA_PIM_ERR_NONE;
}

/**
 * Release all the objects.
 */
- (void) dealloc
{
    
    [mEventsDictionary release];
    
    [super dealloc];
}


@end
