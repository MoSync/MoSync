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

// Default maximum value for the progress bar.
#define DEFAULT_MAXIMUM_VALUE 100

// Default progress value for the progress bar.
#define DEFAULT_PROGRESS_VALUE 0

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "ProgressBarWidget.h"

@implementation ProgressBarWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if(self)
    {
        self.view = [[[UIProgressView alloc] initWithProgressViewStyle: UIProgressViewStyleBar] autorelease];
    }

    _maxValue = DEFAULT_MAXIMUM_VALUE;
    _progressValue = DEFAULT_PROGRESS_VALUE;

    return self;
}

/**
 * Sets a progress bar property.
 * @param key The property of the progress bar widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey:(NSString*)key toValue:(NSString*)value
{
    UIProgressView* progressView = (UIProgressView*) self.view;
    float paramValue = [value floatValue];

    if([key isEqualToString:@MAW_PROGRESS_BAR_MAX])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _maxValue = paramValue;

        // Check if progress bar's value is bigger then the new maximum upper range.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
        }

        // Set the new value for the progress bar.
        float currentPercentage = _progressValue / _maxValue;
        [progressView setProgress:currentPercentage];
    }
    else if([key isEqualToString:@MAW_PROGRESS_BAR_PROGRESS])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _progressValue = [value floatValue];

        // Check if the new progress value is bigger then the maximum upper range.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
        }

        // Set the new progress value for the progress bar.
        if (0 != _maxValue)
        {
            float currentPercent = _progressValue / _maxValue;
            [progressView setProgress:currentPercent];
        }
    }
    else if([key isEqualToString:@MAW_PROGRESS_BAR_INCREMENT_PROGRESS])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        _progressValue += [value floatValue];

        // Check if the new progress value is bigger then the maximum upper range.
        if (_progressValue > _maxValue)
        {
            _progressValue = _maxValue;
        }

        // Increment the progress value for the progress bar.
        if (0 != _maxValue)
        {
            float currentPercent = _progressValue / _maxValue;
            [progressView setProgress:currentPercent];
        }
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

    return MAW_RES_OK;
}

/**
 * Returns a property value of the progress bar widget.
 * @param key The property of the progress bar widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey:(NSString*)key
{
	if([key isEqualToString:@MAW_PROGRESS_BAR_MAX])
    {
        // Return the maximum value for the progress bar.
		return [[NSString alloc] initWithFormat:@"%d", (int)_maxValue];
	}
    else if([key isEqualToString:@MAW_PROGRESS_BAR_PROGRESS])
    {
        // Return the progress value of the progress bar.
        return [[NSString alloc] initWithFormat:@"%d", (int)_progressValue];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

@end
