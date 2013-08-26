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

#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

#import "ActivityIndicatorWidget.h"
#import "WidgetUtils.h"

@implementation ActivityIndicatorWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        UIActivityIndicatorView* activityIndicatorView =
            [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle: UIActivityIndicatorViewStyleGray];

        self.view = activityIndicatorView;
        [activityIndicatorView startAnimating];
        [activityIndicatorView release];
        activityIndicatorView = NULL;
    }
    return self;
}

/**
 * Sets a activity indicator property.
 * @param key The property of the activity indicator widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    if([key isEqualToString:@MAW_ACTIVITY_INDICATOR_IN_PROGRESS])
    {
        UIActivityIndicatorView* activityIndicatorView = (UIActivityIndicatorView*) self.view;

        if([value isEqualToString: kWidgetTrueValue])
        {
            [activityIndicatorView startAnimating];
        }
        else if([value isEqualToString: kWidgetFalseValue])
        {
            [activityIndicatorView stopAnimating];
        }
        else
        {
            return MAW_RES_INVALID_PROPERTY_VALUE;
        }
    }
    else
    {
        return [super setPropertyWithKey:key toValue:value];
    }

	return MAW_RES_OK;
}

@end
