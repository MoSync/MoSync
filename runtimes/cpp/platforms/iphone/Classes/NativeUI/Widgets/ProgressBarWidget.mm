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
