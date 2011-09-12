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

#import "TimePickerWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#include "Platform.h"

@implementation TimePickerWidget

/**
 * Init function.
 */
- (id)init
{
    if (!view)
    {
        UIDatePicker* datePicker = [[UIDatePicker alloc] init];
        [datePicker setDatePickerMode:UIDatePickerModeTime];
        [datePicker addTarget:self action:@selector(valueChanged:)
             forControlEvents:UIControlEventValueChanged];

        // Local settings must be set to UK settings in order to hide
        // the AM/PM option.
        NSLocale* locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_UK"];;
        [datePicker setLocale:locale];

        view = datePicker;
    }

    return [super init];
}

/**
 * Sets a time picker property.
 * @param key The property of the time picker widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    UIDatePicker* timePicker = (UIDatePicker*) view;
    int paramValue = [value intValue];

    if ([key isEqualToString:@MAW_TIME_PICKER_CURRENT_HOUR] ||
        [key isEqualToString:@MAW_TIME_PICKER_CURRENT_MINUTE])
    {
        // Set a specified time into time picker.
        unsigned unitFlags = NSHourCalendarUnit | NSMinuteCalendarUnit;
        NSDate* selectedDate = [timePicker date];
        NSCalendar* defaultCalendar = [NSCalendar currentCalendar];
        NSDateComponents *components = [defaultCalendar components:unitFlags fromDate:selectedDate];

        if ([key isEqualToString:@MAW_TIME_PICKER_CURRENT_HOUR])
        {
            // Check if the param is a valid hour.
            if (paramValue < 0 || paramValue > 23)
            {
                return MAW_RES_INVALID_PROPERTY_VALUE;
            }

            [components setHour:paramValue];
        }
        else if ([key isEqualToString:@MAW_TIME_PICKER_CURRENT_MINUTE])
        {
            // Check if the param is a valid minute.
            if (paramValue < 0 || paramValue > 59)
            {
                return MAW_RES_INVALID_PROPERTY_VALUE;
            }

            [components setMinute:paramValue];
        }

        NSDate* newDate = [defaultCalendar dateFromComponents:components];
        [timePicker setDate:newDate];
    }
    else
    {
       return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the time picker widget.
 * @param key The property of the time picker widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    UIDatePicker* timePicker = (UIDatePicker*) view;

    if([key isEqualToString:@MAW_TIME_PICKER_CURRENT_HOUR])
    {
        // Return the selected month from the date picker.
        NSDate* date = [timePicker date];
        NSDateComponents *components = [[NSCalendar currentCalendar] components:kCFCalendarUnitHour fromDate:date];
        NSInteger hour = [components hour];
        return[[NSString alloc] initWithFormat:@"%d", hour];
	}
    else if([key isEqualToString:@MAW_TIME_PICKER_CURRENT_MINUTE])
    {
        // Return the selected day from the date picker.
        NSDate* date = [timePicker date];
        NSDateComponents *components = [[NSCalendar currentCalendar] components:kCFCalendarUnitMinute fromDate:date];
        NSInteger minute = [components minute];
        return[[NSString alloc] initWithFormat:@"%d", minute];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Notifier for time picker.
 * Called each time when user modifies the time.
 * @param sender
 */
-(void) valueChanged:(id) sender
{
    UIDatePicker* timePicker = (UIDatePicker*) view;
    NSDate* date = [timePicker date];
    NSDateComponents *components = [[NSCalendar currentCalendar]
                                    components:kCFCalendarUnitHour|kCFCalendarUnitMinute
                                    fromDate:date];
    NSInteger hour = [components hour];
    NSInteger minute = [components minute];

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_TIME_PICKER_VALUE_CHANGED;
    eventData->hour = hour;
    eventData->minute = minute;
	eventData->widgetHandle = handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
