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

#import "ProgressBarWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation ProgressBarWidget

/**
 * Init function.
 */
- (id)init
{
    if(!view)
    {
        //UIProgressView* progressView = [[UIProgressView alloc] init];
        view = [[[UIProgressView alloc] initWithProgressViewStyle: UIProgressViewStyleBar] retain];
    }

    mMaxValue = DEFAULT_MAXIMUM_VALUE;
    mProgressValue = DEFAULT_PROGRESS_VALUE;

    return [super init];
}

/**
 * Sets a progress bar property.
 * @param key The property of the progress bar widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    UIProgressView* progressView = (UIProgressView*) view;
    float paramValue = [value floatValue];

    if([key isEqualToString:@"max"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mMaxValue = paramValue;

        // Check if progress bar's value is bigger then the new maximum upper range.
        if(mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
        }

        // Set the new value for the progress bar.
        float currentPercentage = mProgressValue / mMaxValue;
        [progressView setProgress:currentPercentage];
    }
    else if([key isEqualToString:@"progress"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mProgressValue = [value floatValue];

        // Check if the new progress value is bigger then the maximum upper range.
        if (mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
        }

        // Set the new progress value for the progress bar.
        if (0 != mMaxValue)
        {
            float currentPercent = mProgressValue / mMaxValue;
            [progressView setProgress:currentPercent];
        }
    }
    else if([key isEqualToString:@"incrementProgress"])
    {
        TEST_FOR_NEGATIVE_VALUE(paramValue);
        mProgressValue += [value floatValue];

        // Check if the new progress value is bigger then the maximum upper range.
        if (mProgressValue > mMaxValue)
        {
            mProgressValue = mMaxValue;
        }

        // Increment the progress value for the progress bar.
        if (0 != mMaxValue)
        {
            float currentPercent = mProgressValue / mMaxValue;
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
- (NSString*)getPropertyWithKey: (NSString*)key
{
	if([key isEqualToString:@"max"])
    {
        // Return the maximum value for the progress bar.
		return [[NSString alloc] initWithFormat:@"%d", (int)mMaxValue];
	}
    else if([key isEqualToString:@"progress"])
    {
        // Return the progress value of the progress bar.
        return [[NSString alloc] initWithFormat:@"%d", (int)mProgressValue];
	}
    else
    {
		return [super getPropertyWithKey:key];
	}
}

@end
