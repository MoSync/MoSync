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

// default maximum value for the progress bar.
#define DEFAULT_MAXIMUM_VALUE 100

// default progress value for the progress bar.
#define DEFAULT_PROGRESS_VALUE 0

#import "ProgressBarWidget.h"
#include <helpers/CPP_IX_WIDGET.h>

@implementation ProgressBarWidget

/**
 * Init function.
 */
- (id)init {

    if(!view) {
        UIProgressView* progressView = [[UIProgressView alloc] init];
        view = [[progressView initWithProgressViewStyle: UIProgressViewStyleBar] retain];
    }

    maxValue = DEFAULT_MAXIMUM_VALUE;
    progressValue = DEFAULT_PROGRESS_VALUE;

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

        result = MAW_RES_OK;

        if([key isEqualToString:@"max"]) {

            maxValue = paramValue;

            // check if progress bar's value is bigger then the new maximum upper range
            if(progressValue > maxValue) {
                progressValue = maxValue;
            }

            // set the new value for the progress bar
            float currentPercentage = progressValue / maxValue;
            UIProgressView* progressView = (UIProgressView*) view;
            [progressView setProgress:currentPercentage];

        } else if([key isEqualToString:@"progress"]) {

            progressValue = [value floatValue];

            // check if the new progress value is bigger then the maximum upper range
            if(progressValue > maxValue) {
                progressValue = maxValue;
            }

            // set the new progress value for the progress bar
            float currentPercent = progressValue / maxValue;
            UIProgressView* progressView = (UIProgressView*) view;
            [progressView setProgress:currentPercent];

        } else if([key isEqualToString:@"incrementProgress"]) {

           progressValue += [value floatValue];

            // check if the new progress value is bigger then the maximum upper range
            if(progressValue > maxValue) {
                progressValue = maxValue;
            }

             // increment the progress value for the progress bar
            float currentPercent = progressValue / maxValue;
            UIProgressView* progressView = (UIProgressView*) view;
            [progressView setProgress:currentPercent];

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
        // return the maximum value for the progress bar
		return [[NSString alloc] initWithFormat:@"%d", (int)maxValue];
	} else if([key isEqualToString:@"progress"]) {
        // return the progress value of the progress bar
        return [[NSString alloc] initWithFormat:@"%d", (int)progressValue];
	} else {
		return [super getPropertyWithKey:key];
	}
}

@end
