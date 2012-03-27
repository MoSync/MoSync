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

/**
 * Default size values for picker.
 */
#define DEFAULT_PICKER_HEIGHT 180
#define DEFAULT_PICKER_WIDTH 240

/**
 * Default maximum and minimum values for date picker.
 */
#define MAX_DATE_YEAR 9999
#define MAX_DATE_MONTH 12
#define MAX_DATE_DAY 31
#define MIN_DATE_YEAR 1
#define MIN_DATE_MONTH 1
#define MIN_DATE_DAY 1

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "DatePickerWidget.h"
#include "Platform.h"
#include "NSStringExpanded.h"

/**
 * Hidden methods/functions for DatePickerWidget class.
 */
@interface DatePickerWidget (hidden)

/**
 * Get NSDateComponents object from a given NSDate object.
 * @param date Given date.
 * @return A NSDateComponents object.
 */
-(NSDateComponents*) dateComponentsFromDate:(NSDate*) date;

/**
 * Get the maximum date supported by date picker.
 * @return The maximum date.
 */
-(NSDate*) maximumDate;

/**
 * Get the minimum date supported by date picker.
 * @return The minimum date.
 */
-(NSDate*) minimumDate;

/**
 * Set the max date year.
 * @param yearString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the yearString value it's invalid.
 */
-(int) setMaxDateYear:(NSString*) yearString;

/**
 * Get the max date year.
 * @return Max date year.
 */
-(NSString*) maxDateYear;

/**
 * Set the max date month.
 * @param monthString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the monthString value it's invalid.
 */
-(int) setMaxDateMonth:(NSString*) monthString;

/**
 * Get the max date month.
 * @return Max date month.
 */
-(NSString*) maxDateMonth;

/**
 * Set the max date day.
 * @param dayString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the dayString value it's invalid.
 */
-(int) setMaxDateDay:(NSString*) dayString;

/**
 * Get the max date day.
 * @return Max date day.
 */
-(NSString*) maxDateDay;

/**
 * Set the min date year.
 * @param yearString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the yearString value it's invalid.
 */
-(int) setMinDateYear:(NSString*) yearString;

/**
 * Get the min date year.
 * @return Min date year.
 */
-(NSString*) minDateYear;

/**
 * Set the min date month.
 * @param monthString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the monthString value it's invalid.
 */
-(int) setMinDateMonth:(NSString*) monthString;

/**
 * Get the min date month.
 * @return Min date month.
 */
-(NSString*) minDateMonth;

/**
 * Set the min date day.
 * @param dayString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the dayString value it's invalid.
 */
-(int) setMinDateDay:(NSString*) dayString;

/**
 * Get the min date day.
 * @return Min date day.
 */
-(NSString*) minDateDay;

/**
 * Set a date unit(day, month or year).
 * @param key One of the next constants:
 * - MAW_DATE_PICKER_DAY_OF_MONTH
 * - MAW_DATE_PICKER_MONTH
 * - MAW_DATE_PICKER_YEAR
 * @param value Date value(a given day, month or year).
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setCurrentDateUnit:(NSString*) key value:(NSString*) value;

/**
 * Set max date for a date picker using a time interval.
 * This property has been deprecated.
 * @param interval Time interval since 1st Jan 1970 in seconds.
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setMaxDateUsingTimeInterval:(double) interval;

/**
 * Set min date for a date picker using a time interval.
 * This property has been deprecated.
 * @param interval Time interval since 1st Jan 1970 in seconds.
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setMinDateUsingTimeInterval:(double) interval;

@end


@implementation DatePickerWidget

/**
 * Init function.
 */
- (id)init
{
    if(!view)
    {
        UIDatePicker* datePicker = [[UIDatePicker alloc] initWithFrame:CGRectMake(0,
                                                                                  0,
                                                                                  DEFAULT_PICKER_WIDTH,
                                                                                  DEFAULT_PICKER_HEIGHT)];
        [datePicker setDatePickerMode:UIDatePickerModeDate];
        [datePicker addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventValueChanged];
        datePicker.maximumDate = [self maximumDate];
        datePicker.minimumDate = [self minimumDate];

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
    double paramValue = [value doubleValue];

    if([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE])
    {
        return [self setMaxDateUsingTimeInterval:paramValue];
    }
    else if([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE])
    {
        return [self setMinDateUsingTimeInterval:paramValue];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_DAY_OF_MONTH] ||
             [key isEqualToString:@MAW_DATE_PICKER_MONTH]      ||
             [key isEqualToString:@MAW_DATE_PICKER_YEAR])
    {
        return [self setCurrentDateUnit:key value:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_YEAR])
    {
        return [self setMaxDateYear:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_MONTH])
    {
        return [self setMaxDateMonth:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_DAY])
    {
        return [self setMaxDateDay:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_YEAR])
    {
        return [self setMinDateYear:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_MONTH])
    {
        return [self setMinDateMonth:value];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_DAY])
    {
        return [self setMinDateDay:value];
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
        return [[NSString alloc] initWithFormat:@"%d", day];
	}
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_YEAR])
    {
        return [[self maxDateYear] retain];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_MONTH])
    {
        return [[self maxDateMonth] retain];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MAX_DATE_DAY])
    {
        return [[self maxDateDay] retain];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_YEAR])
    {
        return [[self minDateYear] retain];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_MONTH])
    {
        return [[self minDateMonth] retain];
    }
    else if ([key isEqualToString:@MAW_DATE_PICKER_MIN_DATE_DAY])
    {
        return [[self minDateDay] retain];
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

/**
 * Implementation for hidden methods/functions.
 */
@implementation DatePickerWidget (hidden)

/**
 * Get NSDateComponents object from a given NSDate object.
 * @param date Given date.
 * @return A NSDateComponents object.
 */
-(NSDateComponents*) dateComponentsFromDate:(NSDate*) date
{
    unsigned unitFlags = NSYearCalendarUnit | NSMonthCalendarUnit |  NSDayCalendarUnit;
    NSCalendar* defaultCalendar = [NSCalendar currentCalendar];
    NSDateComponents *components = [defaultCalendar components:unitFlags fromDate:date];
    return components;
}

/**
 * Get the maximum date supported by date picker.
 * @return The maximum date.
 */
-(NSDate*) maximumDate
{
    NSDate* currentDate = [NSDate date];
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentDate];
    dateComponents.year = MAX_DATE_YEAR;
    dateComponents.month = MAX_DATE_MONTH;
    dateComponents.day = MAX_DATE_DAY;
    NSDate* maxDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    return maxDate;
}

/**
 * Get the minimum date supported by date picker.
 * @return The minimum date.
 */
-(NSDate*) minimumDate
{
    NSDate* currentDate = [NSDate date];
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentDate];
    dateComponents.year = MIN_DATE_YEAR;
    dateComponents.month = MIN_DATE_MONTH;
    dateComponents.day = MIN_DATE_DAY;
    NSDate* minDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    return minDate;
}

/**
 * Set the max date year.
 * @param yearString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the yearString value it's invalid.
 */
-(int) setMaxDateYear:(NSString*) yearString
{
    if(![yearString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger year = [yearString intValue];
    // Check if year value is valid.
    if (year < MIN_DATE_YEAR || year > MAX_DATE_YEAR)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMaxDate = datePicker.maximumDate;

    // Set the max year value.
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMaxDate];
    dateComponents.year = year;
    NSDate* maxDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.maximumDate = maxDate;
    return MAW_RES_OK;
}

/**
 * Get the max date year.
 * @return Max date year.
 */
-(NSString*) maxDateYear
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.maximumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSYearCalendarUnit fromDate:date];
    NSInteger year = [components year];
    return [NSString stringWithFormat:@"%d", year];
}

/**
 * Set the max date month.
 * @param monthString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the monthString value it's invalid.
 */
-(int) setMaxDateMonth:(NSString*) monthString
{
    if(![monthString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger month = [monthString intValue];
    // Check if month value is valid.
    if (month < MIN_DATE_MONTH || month > MAX_DATE_MONTH)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMaxDate = datePicker.maximumDate;

    // Set the max month value.
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMaxDate];
    dateComponents.month = month;
    NSDate* maxDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.maximumDate = maxDate;
    return MAW_RES_OK;
}

/**
 * Get the max date month.
 * @return Max date month.
 */
-(NSString*) maxDateMonth
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.maximumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSMonthCalendarUnit fromDate:date];
    NSInteger month = [components month];
    return [NSString stringWithFormat:@"%d", month];
}

/**
 * Set the max date day.
 * @param dayString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the dayString value it's invalid.
 */
-(int) setMaxDateDay:(NSString*) dayString
{
    if(![dayString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger day = [dayString intValue];
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMaxDate = datePicker.maximumDate;
    // If max date value is not set(the value is nil) set the default value.
    if (!currentMaxDate)
    {
        currentMaxDate = [self maximumDate];
    }

    // Check if day is valid.
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMaxDate];
    int oldMaxDay =  dateComponents.day;
    dateComponents.day = 1;
    NSRange range = [[NSCalendar currentCalendar]
                     rangeOfUnit:NSDayCalendarUnit
                     inUnit:NSMonthCalendarUnit
                     forDate:currentMaxDate];
    if (day < MIN_DATE_DAY || day > range.length)
    {
        // Restore previousday value and return error code.
        dateComponents.day = oldMaxDay;
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    // Set the max day.
    dateComponents.day = day;
    NSDate* maxDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.maximumDate = maxDate;
    return MAW_RES_OK;
}

/**
 * Get the max date day.
 * @return Max date day.
 */
-(NSString*) maxDateDay
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.maximumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSDayCalendarUnit fromDate:date];
    NSInteger day = [components day];
    return [NSString stringWithFormat:@"%d", day];
}

/**
 * Set the min date year.
 * @param yearString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the yearString value it's invalid.
 */
-(int) setMinDateYear:(NSString*) yearString
{
    if(![yearString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger year = [yearString intValue];
    // Check if year value is valid.
    if (year < MIN_DATE_YEAR || year > MAX_DATE_YEAR)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMinDate = datePicker.minimumDate;

    // Set the min year value.
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMinDate];
    dateComponents.year = year;
    NSDate* minDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.minimumDate = minDate;
    return MAW_RES_OK;
}

/**
 * Get the min date year.
 * @return Min date year.
 */
-(NSString*) minDateYear
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.minimumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSYearCalendarUnit fromDate:date];
    NSInteger year = [components year];
    return [NSString stringWithFormat:@"%d", year];
}

/**
 * Set the min date month.
 * @param monthString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the monthString value it's invalid.
 */
-(int) setMinDateMonth:(NSString*) monthString
{
    if(![monthString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger month = [monthString intValue];
    // Check if month value is valid.
    if (month < MIN_DATE_MONTH || month > MAX_DATE_MONTH)
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMinDate = datePicker.minimumDate;

    // Set the min month value.
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMinDate];
    dateComponents.month = month;
    NSDate* minDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.minimumDate = minDate;
    return MAW_RES_OK;
}

/**
 * Get the min date month.
 * @return Min date month.
 */
-(NSString*) minDateMonth
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.minimumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSMonthCalendarUnit fromDate:date];
    NSInteger month = [components month];
    return [NSString stringWithFormat:@"%d", month];
}

/**
 * Set the min date day.
 * @param dayString Value that will be set.
 * @return One of the following constants:
 * - MAW_RES_OK if the value was set.
 * - MAW_RES_INVALID_PROPERTY_VALUE if the dayString value it's invalid.
 */
-(int) setMinDateDay:(NSString*) dayString
{
    if(![dayString canParseNumber])
    {
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    NSInteger day = [dayString intValue];
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* currentMinDate = datePicker.minimumDate;
    NSDateComponents* dateComponents = [self dateComponentsFromDate:currentMinDate];

    int oldMinDay =  dateComponents.day;
    dateComponents.day = 1;
    NSRange range = [[NSCalendar currentCalendar]
                     rangeOfUnit:NSDayCalendarUnit
                     inUnit:NSMonthCalendarUnit
                     forDate:currentMinDate];
    if (day < MIN_DATE_DAY || day > range.length)
    {
        // Restore previous day value and return error code.
        dateComponents.day = oldMinDay;
        return MAW_RES_INVALID_PROPERTY_VALUE;
    }

    // Set the min day value.
    dateComponents.day = day;
    NSDate* minDate = [[NSCalendar currentCalendar] dateFromComponents:dateComponents];
    datePicker.minimumDate = minDate;
    return MAW_RES_OK;
}

/**
 * Get the min date day.
 * @return Min date day.
 */
-(NSString*) minDateDay
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* date = datePicker.minimumDate;
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSDayCalendarUnit fromDate:date];
    NSInteger day = [components day];
    return [NSString stringWithFormat:@"%d", day];
}

/**
 * Set a date unit(day, month or year).
 * @param key One of the next constants:
 * - MAW_DATE_PICKER_DAY_OF_MONTH
 * - MAW_DATE_PICKER_MONTH
 * - MAW_DATE_PICKER_YEAR
 * @param value Date value(a given day, month or year).
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setCurrentDateUnit:(NSString*) key value:(NSString*) value
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    double paramValue = [value doubleValue];
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
    return MAW_RES_OK;
}

/**
 * Set max date for a date picker using a time interval.
 * This property has been deprecated.
 * @param interval Time interval since 1st Jan 1970 in seconds.
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setMaxDateUsingTimeInterval:(double) interval
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* maxDate = [NSDate dateWithTimeIntervalSince1970:interval];
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

    return MAW_RES_OK;
}

/**
 * Set min date for a date picker using a time interval.
 * This property has been deprecated.
 * @param interval Time interval since 1st Jan 1970 in seconds.
 * @return MAW_RES_OK if no error occured, or a error code otherwise.
 */
-(int) setMinDateUsingTimeInterval:(double) interval
{
    UIDatePicker* datePicker = (UIDatePicker*) view;
    NSDate* minDate = [NSDate dateWithTimeIntervalSince1970:interval];
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

    return MAW_RES_OK;
}

@end
