/* Copyright (C) 2011 MoSync AB

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

#import "DatePickerWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#include "Platform.h"

@implementation DatePickerWidget

/**
 * Init function.
 */
- (id)init
{
    if(!view)
    {
        UIDatePicker* datePicker = [[UIDatePicker alloc] init];
        [datePicker setDatePickerMode:UIDatePickerModeDate];
        [datePicker addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventValueChanged];

        view = datePicker;
    }

    return [super init];
}

/**
 * Sets a date picker property.
 * @param key The property of the date picker widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    double paramValue = [value doubleValue];

    if([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE])
    {
        // Set a maximum date into date picker.
        NSDate* maxDate = [NSDate dateWithTimeIntervalSince1970:paramValue];
        NSDate* minDate = [datePicker minimumDate];

        // If the max date is earlier then min date don't set the value.
        if (NSOrderedAscending == [maxDate compare:minDate])
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        else
        {
            [datePicker setMaximumDate:maxDate];
        }
    }
    else if([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE])
    {
        // Set a minimum date into date picker.
        NSDate* minDate = [NSDate dateWithTimeIntervalSince1970:paramValue];
        NSDate* maxDate = [datePicker maximumDate];

        // If the min date is later then max date don't set the value.
        if (NSOrderedDescending == [minDate compare:maxDate])
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        else
        {
            [datePicker setMinimumDate:minDate];
        }
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_DAY_OF_MONTH] ||
             [key isEqualToString:@MAW_DATE_PICKER_MONTH]      ||
             [key isEqualToString:@MAW_DATE_PICKER_YEAR])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);

        // Set a specified date into date picker.
        unsigned unitFlags = NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit;
        NSDate* selectedDate = [datePicker date];
        NSCalendar* defaultCalendar = [NSCalendar currentCalendar];
        NSDateComponents *components = [defaultCalendar components:unitFlags fromDate:selectedDate];

        if ([key isEqualToString:@MAW_DATE_PICKER_DAY_OF_MONTH])
        {
            [components setDay:paramValue];
        }
        else if ([key isEqualToString:@MAW_DATE_PICKER_MONTH])
        {
            [components setMonth:paramValue];
        }
        else if ([key isEqualToString:@MAW_DATE_PICKER_YEAR])
        {
            [components setYear:paramValue];
        }

        NSDate* newDate = [defaultCalendar dateFromComponents:components];
        [datePicker setDate:newDate];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the date picker widget.
 * @param key The property of the date picker widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{

    UIDatePicker* datePicker = (UIDatePicker*) view;
	if([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE])
    {
        // return the maximum date for the date picker.
        NSDate* maxDate = [datePicker maximumDate];
        NSTimeInterval timeInterval = [maxDate timeIntervalSince1970];
        return[[NSString alloc] initWithFormat:@"%d", (int)timeInterval];
	}
    else if([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE])
    {
        // return the minimum date for the date picker.
        NSDate* minDate = [datePicker minimumDate];
        NSTimeInterval timeInterval = [minDate timeIntervalSince1970];
        return[[NSString alloc] initWithFormat:@"%d", (int)timeInterval];

	}
    else if([key isEqualToString:@MAW_DATE_PICKER_YEAR])
    {
        // return the selected year from the date picker.
        NSDate* date = [datePicker date];
        NSDateComponents *components = [[NSCalendar currentCalendar] components:NSYearCalendarUnit fromDate:date];
        NSInteger year = [components year];
        return[[NSString alloc] initWithFormat:@"%d", year];
	}
    else if([key isEqualToString:@MAW_DATE_PICKER_MONTH])
    {
        // return the selected month from the date picker.
        NSDate* date = [datePicker date];
        NSDateComponents *components = [[NSCalendar currentCalendar] components:kCFCalendarUnitMonth fromDate:date];
        NSInteger month = [components month];
        return[[NSString alloc] initWithFormat:@"%d", month];
	}
    else if([key isEqualToString:@MAW_DATE_PICKER_DAY_OF_MONTH])
    {
        // return the selected day from the date picker.
        NSDate* date = [datePicker date];
        NSDateComponents *components = [[NSCalendar currentCalendar] components:kCFCalendarUnitDay fromDate:date];
        NSInteger day = [components day];
        return[[NSString alloc] initWithFormat:@"%d", day];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Notifier for date picker.
 * Called each time when user modifies the date.
 * @param sender
 */
-(void) valueChanged:(id) sender
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = [datePicker date];
    NSDateComponents *components = [[NSCalendar currentCalendar]
                                    components:kCFCalendarUnitDay|kCFCalendarUnitMonth|kCFCalendarUnitYear
                                    fromDate:date];
    NSInteger day = [components day];
    NSInteger month = [components month];
    NSInteger year = [components year];

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_DATE_PICKER_VALUE_CHANGED;
    eventData->dayOfMonth = day;
    eventData->month = month;
    eventData->year = year;
	eventData->widgetHandle = handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
