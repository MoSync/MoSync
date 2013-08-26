/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
