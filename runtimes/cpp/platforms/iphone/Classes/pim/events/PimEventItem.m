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


#import "PimEventItem.h"


@implementation PimEventItem

/**
 * Init function.
 */
-(id) init
{
    return [super init];
}

/**
 * Init function with Calendar event.
 * @param event Calendar event.
 */
-(id) initWithEvent:(EKEvent*) event
{
    mEvent = event;
    PimFieldItem* itemField;
    NSString* key;
    NSMutableArray* array;
    [super init];
    
    // Add event title field.
    NSString* title = [mEvent title];
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_EVENT_TITLE];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_EVENT_TITLE];
    array = [[NSMutableArray alloc] init];
    [array addObject:title];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"event title: %@", title);
    
    // Add event start date field.
    NSDate* startDate = [mEvent startDate];
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_EVENT_START_DATE];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_EVENT_START_DATE];
    array = [[NSMutableArray alloc] init];
    [array addObject:startDate];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"event start date: %f", [startDate timeIntervalSince1970]);
    
    // Add event end date field.
    NSDate* endDate = [mEvent endDate];
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_EVENT_END_DATE];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_EVENT_END_DATE];
    array = [[NSMutableArray alloc] init];
    [array addObject:endDate];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"event start date: %f", [endDate timeIntervalSince1970]);
    
    // Add event location field.
    NSString* location = [mEvent location];
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_EVENT_LOCATION];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_EVENT_LOCATION];
    array = [[NSMutableArray alloc] init];
    [array addObject:location];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"event location: %@", location);
    
    // Add event description field.
    NSString* description = [mEvent notes];
    itemField = [[PimFieldItem alloc] initWithFieldConstant:MA_PIM_FIELD_EVENT_DESCRIPTION];
    key = [[NSString alloc] initWithFormat:@"%d", MA_PIM_FIELD_EVENT_DESCRIPTION];
    array = [[NSMutableArray alloc] init];
    [array addObject:description];
    [itemField addValue:array withAttribute:0];
    [mFieldsDictionary setObject:itemField forKey:key];
    NSLog(@"event description: %@", description);
    
    return self;
}

/**
 * Writes all the fields into record.
 * Deletes all item's fields.
 * @return One of the MA_PIM_ERR constants.
 */
-(int) close
{
    return 0;
}

@end
