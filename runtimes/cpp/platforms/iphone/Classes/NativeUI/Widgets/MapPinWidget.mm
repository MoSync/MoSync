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

#import "MapPinWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation MapPinWidget

/**
 * Init function.
 */
- (id)init
{
    self = [super init];
    if (self)
    {
        annotation = [[MapAnnotation alloc] init];
        MKAnnotationView* annotationView = [[MKAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"mapAnnotation"];
        self.view = annotationView;
        [annotationView release];
        annotationView = nil;
    }

    return self;
}

/**
 * Sets a map pin property.
 * @param key The property of the map pin widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    if([key isEqualToString:@MAW_MAP_PIN_LATITUDE])
    {
        annotationCoordinate.latitude = [value doubleValue];
		annotation.coordinate = annotationCoordinate;
	}
	else if ([key isEqualToString:@MAW_MAP_PIN_LONGITUDE])
	{
        annotationCoordinate.longitude = [value doubleValue];
		annotation.coordinate = annotationCoordinate;
	}
	else if ([key isEqualToString:@MAW_MAP_PIN_TEXT])
	{
        annotation.title = value;
	}

    return MAW_RES_OK;
}

/**
 * Returns a property value of the map pin widget.
 * @param key The property of the map pin widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
    if([key isEqualToString:@MAW_MAP_PIN_LATITUDE])
    {
		return [[NSString alloc] initWithFormat:@"%f", annotation.coordinate.latitude];
	}
	else if ([key isEqualToString:@MAW_MAP_PIN_LONGITUDE])
	{
		return [[NSString alloc] initWithFormat:@"%f", annotation.coordinate.longitude];
	}
	else if ([key isEqualToString:@MAW_MAP_PIN_TEXT])
	{
        return [[NSString alloc] initWithString:annotation.title];
	}

	return @"";
}

/**
 * Returns the annotation view that will be added on the map.
 */
- (MapAnnotation*)getAnnotation
{
    return annotation;
}

@end
