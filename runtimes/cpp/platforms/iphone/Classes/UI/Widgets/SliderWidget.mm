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

// default maximum value for the slider.
#define DEFAULT_MAXIMUM_VALUE 100

// default minimum value for the slider.
#define DEFAULT_MINIMUM_VALUE 0

#import "SliderWidget.h"
#include <helpers/CPP_IX_WIDGET.h>
#include <helpers/cpp_defs.h>
#include "Platform.h"

@implementation SliderWidget

/**
 * Init function.
 */
- (id)init {
    
    if(!view) {
        UISlider* slider = [[UISlider alloc] init];
    	view = [slider retain];
        [slider addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventTouchDragInside];
    }
	
    maxValue = DEFAULT_MAXIMUM_VALUE;
    progressValue = DEFAULT_MINIMUM_VALUE;
    
    return [super init];
}

/**
 * Sets a progress bar property.
 * @param key The property of the progress bar widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value {
	
    int result = [super setPropertyWithKey:key toValue:value];
    
    if(MAW_RES_OK != result) {
        
        // check for negative values
        float paramValue = [value floatValue];
        if(0 > paramValue) {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
        
        UISlider* slider = (UISlider*) view;
        result = MAW_RES_OK;
        
        if([key isEqualToString:@"max"]) {
            
            // set the upper range(max) value
            maxValue = paramValue;
            [slider setMaximumValue:maxValue];
            
            // check if slider's value is bigger then the new maximum upper range
            if(progressValue > maxValue) {
                progressValue = maxValue;
                [slider setValue:progressValue];
            }
            
        } else if([key isEqualToString:@"value"]) {
            
            progressValue = paramValue;
            
            // check if the new value if bigger then the maximum value
            if(progressValue > maxValue) {
                progressValue = maxValue;
            }
            
            // set the new value for the slider
            [slider setValue:progressValue];
            
        } else if([key isEqualToString:@"incrementValue"]) {
            
            progressValue += paramValue;
           
            // check if the new value if bigger then the maximum value
            if(progressValue > maxValue) {
                progressValue = maxValue;
            }
            
            // set the new value for the slider
            [slider setValue:progressValue];
            
        } else if([key isEqualToString:@"decrementValue"]) {
            
            progressValue -= paramValue;
            
            // check if the new value if smaller then the minimum value
            if(progressValue < DEFAULT_MINIMUM_VALUE) {
                progressValue = DEFAULT_MINIMUM_VALUE;
            }
            
            // set the new value for the slider
            [slider setValue:progressValue];
            
        } else {
            result = MAW_RES_INVALID_PROPERTY_NAME;
        }
        
    }
    
    return result;	
}

/**
 * Returns a property value of the progress bar widget.
 * @param key The property of the progress bar widget. 
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key {
    
	if([key isEqualToString:@"max"]) {
        // return the maximum value for the slider
         return[[NSString alloc] initWithFormat:@"%d", (int)maxValue];
	} else if([key isEqualToString:@"value"]) {
        // return the value of the slider
        return[[NSString alloc] initWithFormat:@"%d", (int)progressValue];
	} else {
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
    progressValue = [slider value];

    MAEvent event;
	event.type = EVENT_TYPE_WIDGET;
    
	MAWidgetEventData *eventData = new MAWidgetEventData;
	eventData->eventType = MAW_EVENT_SLIDER_VALUE_CHANGED;
    eventData->sliderValue = (int) progressValue;
	eventData->widgetHandle = handle;
	
    event.data = (int)eventData;
    Base::gEventQueue.put(event);
}

@end
