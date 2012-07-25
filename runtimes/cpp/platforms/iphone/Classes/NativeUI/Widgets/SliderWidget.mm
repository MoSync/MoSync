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

// Default maximum value for the slider.
#define DEFAULT_MAXIMUM_VALUE 100

// Default minimum value for the slider.
#define DEFAULT_MINIMUM_VALUE 0

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "SliderWidget.h"
#include "Platform.h"

@implementation SliderWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if(self)
    {
        UISlider* slider = [[UISlider alloc] init];
        self.view = slider;
        [slider addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventTouchDragInside];
        [slider release];
        slider = NULL;
    }

    _maxValue = DEFAULT_MAXIMUM_VALUE;
    _progressValue = DEFAULT_MINIMUM_VALUE;

    return self;
}

/**
 * Sets a slider property.
 * @param key The property of the slider widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    UISlider* slider = (UISlider*) self.view;
    float paramValue = [value floatValue];

    if ([key isEqualToString:@MAW_SLIDER_MAX])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);

        // Set the upper range(max) value.
        _maxValue = paramValue;
        [slider setMaximumValue:_maxValue];

        // Check if slider's value is bigger then the new maximum upper range.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
            [slider setValue:_progressValue];
        }
    }
    else if ([key isEqualToString:@MAW_SLIDER_VALUE])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _progressValue = paramValue;

        // Check if the new value if bigger then the maximum value.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
        }

        // Set the new value for the slider.
        [slider setValue:_progressValue];
    }
    else if ([key isEqualToString:@MAW_SLIDER_INCREASE_VALUE])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _progressValue += paramValue;

        // Check if the new value if bigger then the maximum value.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
        }

        // Set the new value for the slider.
        [slider setValue:_progressValue];
    }
    else if ([key isEqualToString:@MAW_SLIDER_DECREASE_VALUE])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _progressValue -= paramValue;

        // Check if the new value if smaller then the minimum value.
        if (_progressValue < DEFAULT_MINIMUM_VALUE)
        {
            _progressValue = DEFAULT_MINIMUM_VALUE;
        }

        // Set the new value for the slider.
        [slider setValue:_progressValue];
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the slider widget.
 * @param key The property of the slider widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if ([key isEqualToString:@MAW_SLIDER_MAX])
    {
        // Return the maximum value for the slider.
        return[[NSString alloc] initWithFormat:@"%d", (int)_maxValue];
	}
    else if ([key isEqualToString:@MAW_SLIDER_VALUE])
    {
        // Return the value of the slider.
        return[[NSString alloc] initWithFormat:@"%d", (int)_progressValue];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

/**
 * Notifier for slider progress value.
 * Called each time when the user drags the slider.
 * @param sender
 */
-(void) valueChanged:(id) sender
{
    UISlider* slider = (UISlider*) self.view;
    _progressValue = [slider value];

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SLIDER_VALUE_CHANGED;
    eventData->sliderValue = (int) _progressValue;
	eventData->widgetHandle = self.handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
