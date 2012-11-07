/* Copyright (C) 2012 MoSync AB

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
