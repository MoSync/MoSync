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

#import "SliderWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#include "Platform.h"

@implementation SliderWidget

/**
 * Init function.
 */
- (id)init
{
    if(!view)
    {
        UISlider* slider = [[UISlider alloc] init];
        view = [slider retain];
        [slider addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventTouchDragInside];
    }

    mMaxValue = DEFAULT_MAXIMUM_VALUE;
    mProgressValue = DEFAULT_MINIMUM_VALUE;

    return [super init];
}

/**
 * Sets a slider property.
 * @param key The property of the slider widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    UISlider* slider = (UISlider*) view;
    float paramValue = [value floatValue];

    if([key isEqualToString:@"max"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);

        // Set the upper range(max) value.
        mMaxValue = paramValue;
        [slider setMaximumValue:mMaxValue];

        // Check if slider's value is bigger then the new maximum upper range.
        if (mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
            [slider setValue:mProgressValue];
        }
    }
    else if([key isEqualToString:@"value"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mProgressValue = paramValue;

        // Check if the new value if bigger then the maximum value.
        if (mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
        }

        // Set the new value for the slider.
        [slider setValue:mProgressValue];
    }
    else if([key isEqualToString:@"increaseValue"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mProgressValue += paramValue;

        // Check if the new value if bigger then the maximum value.
        if (mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
        }

        // Set the new value for the slider.
        [slider setValue:mProgressValue];
    }
    else if([key isEqualToString:@"decreaseValue"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mProgressValue -= paramValue;

        // Check if the new value if smaller then the minimum value.
        if (mProgressValue < DEFAULT_MINIMUM_VALUE)
        {
            mProgressValue = DEFAULT_MINIMUM_VALUE;
        }

        // Set the new value for the slider.
        [slider setValue:mProgressValue];
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
- (NSString*)getPropertyWithKey: (NSString*)key {

	if([key isEqualToString:@"max"])
    {
        // Return the maximum value for the slider.
        return[[NSString alloc] initWithFormat:@"%d", (int)mMaxValue];
	}
    else if([key isEqualToString:@"value"])
    {
        // Return the value of the slider.
        return[[NSString alloc] initWithFormat:@"%d", (int)mProgressValue];
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
    UISlider* slider = (UISlider*) view;
    mProgressValue = [slider value];

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;

	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SLIDER_VALUE_CHANGED;
    eventData->sliderValue = (int) mProgressValue;
	eventData->widgetHandle = handle;

    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
