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


#import "NumberPickerWidget.h"
#include <helpers/CPP_IX_WIDGET.h>
#include <helpers/cpp_defs.h>
#include "Platform.h"

@implementation NumberPickerWidget

/**
 * Init function.
 */
- (id)init {

    if(!view) {
        UIPickerView* numberPicker = [[UIPickerView alloc] init];
        view = [numberPicker retain];

        [numberPicker setDelegate:self];
        [numberPicker setDataSource:self];
        [numberPicker setShowsSelectionIndicator:true];

        mMaxValue = 0;
        mMinValue = 0;
    }

    return [super init];
}

/**
 * Sets a number picker property.
 * @param key The property of the number picker widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {

    int result = [super setPropertyWithKey:key toValue:value];

    if(MAW_RES_OK != result) {

        UIPickerView* numberPicker = (UIPickerView*) view;
        result = MAW_RES_OK;
        int paramValue = [value intValue];

        if ([key isEqualToString:@"minValue"]) {
            // Check if the param is valid.
            if (paramValue > mMaxValue) {
                return MAW_RES_INVALID_PROPERTY_VALUE;
            }

            mMinValue = paramValue;
            [numberPicker reloadComponent:0];

        } else if ([key isEqualToString:@"maxValue"]) {
            // Check if the param is valid.
            if (paramValue < mMinValue) {
                return MAW_RES_INVALID_PROPERTY_VALUE;
            }

            mMaxValue = paramValue;
            [numberPicker reloadComponent:0];

        } else if ([key isEqualToString:@"value"]) {
            // Check if the param is valid.
            if (mMinValue > paramValue ||  paramValue > mMaxValue) {
                return MAW_RES_INVALID_PROPERTY_VALUE;
            }

            int selectRowIndex = abs(mMinValue - paramValue);
            [numberPicker selectRow:selectRowIndex inComponent:0 animated:false];

        } else {
            result = MAW_RES_INVALID_PROPERTY_NAME;
        }

    }
    return result;
}

/**
 * Returns a property value of the time picker widget.
 * @param key The property of the time picker widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key {

    UIPickerView* numberPicker = (UIPickerView*) view;

    if([key isEqualToString:@"minValue"]) {
        return [[NSString alloc] initWithFormat:@"%d",mMinValue];

	} else if([key isEqualToString:@"maxValue"]) {
        return [[NSString alloc] initWithFormat:@"%d",mMaxValue];

	} else if([key isEqualToString:@"value"]) {
        int selectedRow = [numberPicker selectedRowInComponent:0];
        return [[NSString alloc] initWithFormat:@"%d",selectedRow];

	} else {
		return [super getPropertyWithKey:key];

	}
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)thePickerView {

    return 1;
}

//PickerViewController.m
- (NSInteger)pickerView:(UIPickerView *)thePickerView numberOfRowsInComponent:(NSInteger)component {
    return (mMaxValue - mMinValue + 1);
}

- (NSString *)pickerView:(UIPickerView *)thePickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return [[NSString alloc] initWithFormat:@"%d", (mMinValue + row)];
}

/**
 * Notifier for number picker.
 * Called each time when user modifies the selected number.
 * @param sender
 */
- (void)pickerView:(UIPickerView *)thePickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED;
    eventData->numberPickerValue = (mMinValue + row);
	eventData->widgetHandle = handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);

}

@end
