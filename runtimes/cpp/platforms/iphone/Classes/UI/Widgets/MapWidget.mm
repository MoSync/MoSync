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

#import "MapWidget.h"
#include <helpers/cpp_defs.h>
#include <helpers/CPP_IX_WIDGET.h>

@implementation MapWidget

/**
 * Init function.
 */
- (id)init
{
    if(!view)
    {
        view = [[MKMapView alloc] init];
    }

    return [super init];
}

/**
 * Sets a map property.
 * @param key The property of the map widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value
{
    MKMapView* mapView = (MKMapView*) view;

    if([key isEqualToString:@MAW_MAP_TYPE])
    {
		int mapType = [value intValue];
		if (mapType == MAW_MAP_TYPE_SATELLITE)
		{
			mapView.mapType = MKMapTypeSatellite;
		}
		else
		{
			mapView.mapType = MKMapTypeStandard;
		}
	}
	else if ([key isEqualToString:@MAW_MAP_ZOOM_LEVEL])
	{
		int zoomLevel = [value intValue];
		// TODO set zoom level
	}
	else if ([key isEqualToString:@MAW_MAP_INTERRACTION_ENABLED])
	{
		Boolean interractionEnabled = [value boolValue];
		if (interractionEnabled)
		{
			mapView.zoomEnabled = TRUE;
			mapView.scrollEnabled = TRUE;
		}
		else
		{
			mapView.zoomEnabled = FALSE;
			mapView.scrollEnabled = FALSE;
		}
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_ZOOM_LEVEL])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED_ON_VISIBLE_AREA])
	{
	}
	else
	{
		return [super setPropertyWithKey:key toValue:value];
	}


    return MAW_RES_OK;
}

/**
 * Returns a property value of the map widget.
 * @param key The property of the map widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key
{
	MKMapView* mapView = (MKMapView*) view;

	if([key isEqualToString:@MAW_MAP_TYPE])
    {

	}
	else if ([key isEqualToString:@MAW_MAP_ZOOM_LEVEL])
	{

	}
	else if ([key isEqualToString:@MAW_MAP_INTERRACTION_ENABLED])
	{

	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_ZOOM_LEVEL])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE])
	{
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED_ON_VISIBLE_AREA])
	{
	}
	else
	{
		return [super getPropertyWithKey:key];
	}

	return @"";
}

@end
