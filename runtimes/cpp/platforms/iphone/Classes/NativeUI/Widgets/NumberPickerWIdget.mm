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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "NumberPickerWidget.h"
#include "Platform.h"

@implementation NumberPickerWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UIPickerView* numberPicker = [[UIPickerView alloc] init];
        self.view = numberPicker;

        [numberPicker setDelegate:self];
        [numberPicker setDataSource:self];
        [numberPicker setShowsSelectionIndicator:true];

        _maxValue = 0;
        _minValue = 0;
        [numberPicker release];
        numberPicker = NULL;
    }

    return self;
}

/**
 * Sets a number picker property.
 * @param key The property of the number picker widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    UIPickerView* numberPicker = (UIPickerView*) self.view;
    int paramValue = [value intValue];

    if ([key isEqualToString:@MAW_NUMBER_PICKER_MIN_VALUE])
    {
        // Check if the param is valid.
        if (paramValue > _maxValue)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }

        _minValue = paramValue;
        [numberPicker reloadComponent:0];
    }
    else if ([key isEqualToString:@MAW_NUMBER_PICKER_MAX_VALUE])
    {
        // Check if the param is valid.
        if (paramValue < _minValue) {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }

        _maxValue = paramValue;
        [numberPicker reloadComponent:0];
    }
    else if ([key isEqualToString:@MAW_NUMBER_PICKER_VALUE])
    {
        // Check if the param is valid.
        if (_minValue > paramValue ||  paramValue > _maxValue)
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }

        int selectRowIndex = abs(_minValue - paramValue);
        [numberPicker selectRow:selectRowIndex inComponent:0 animated:false];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the number picker widget.
 * @param key The property of the number picker widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
    UIPickerView* numberPicker = (UIPickerView*) self.view;

    if([key isEqualToString:@MAW_NUMBER_PICKER_MIN_VALUE])
    {
        return [[NSString alloc] initWithFormat:@"%d",_minValue];
	}
    else if([key isEqualToString:@MAW_NUMBER_PICKER_MAX_VALUE])
    {
        return [[NSString alloc] initWithFormat:@"%d",_maxValue];
	}
    else if([key isEqualToString:@MAW_NUMBER_PICKER_VALUE])
    {
        int selectedRow = [numberPicker selectedRowInComponent:0];
        return [[NSString alloc] initWithFormat:@"%d",selectedRow];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)thePickerView
{
    return 1;
}

/**
 * Called by the number picker object when it needs the number of rows for a specified component.
 * @param thePickerView The picker view that is requesting the data.
 * @param component The specified component.
 */
- (NSInteger)pickerView:(UIPickerView *)thePickerView numberOfRowsInComponent:(NSInteger)component
{
    return (_maxValue - _minValue + 1);
}

/**
 * Called by the picker view when it needs the title to use for a given row in a given component.
 * @param thePickerView The picker view object that is requesting the data.
 * @param row A zero-indexed number identifying a row of component.
 * @param component A zero-indexed number identifying a component of picker view.
 */
- (NSString *)pickerView:(UIPickerView *)thePickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [NSString stringWithFormat:@"%d", (_minValue + row)];
}

/**
 * Called by the picker view when the user selects a row in a component.
 * @param thePickerView The picker view object that is requesting the data.
 * @param row A zero-indexed number identifying a row of component.
 * @param component A zero-indexed number identifying a component of pickerView.
 */
- (void)pickerView:(UIPickerView *)thePickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED;
    eventData->numberPickerValue = (_minValue + row);
	eventData->widgetHandle = self.handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
