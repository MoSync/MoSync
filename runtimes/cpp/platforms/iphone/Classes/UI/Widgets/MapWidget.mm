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

#define MERCATOR_OFFSET 268435456
#define MERCATOR_RADIUS 85445659.44705395

@interface MapWidget()

- (double)longitudeToPixelSpaceX:(double)longitude;
- (double)latitudeToPixelSpaceY:(double)latitude;
- (double)pixelSpaceXToLongitude:(double)pixelX;
- (double)pixelSpaceYToLatitude:(double)pixelY;
- (MKCoordinateSpan)coordinateSpanWithMapView:(MKMapView *)mapView
							 centerCoordinate:(CLLocationCoordinate2D)centerCoordinate
								 andZoomLevel:(NSUInteger)zoomLevel;
- (void)setCenterCoordinate:(CLLocationCoordinate2D)centerCoordinate
				  zoomLevel:(NSUInteger)zoomLevel
				   animated:(BOOL)animated;
- (int)getMapViewZoomLevel;

@end


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
		[self setCenterCoordinate:mapView.centerCoordinate zoomLevel:zoomLevel animated:YES];
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
		double latitude = [value doubleValue];
		centerCoordinates.latitude = latitude;
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_LONGITUDE])
	{
		double longitude = [value doubleValue];
		centerCoordinates.longitude = longitude;
	}
	else if ([key isEqualToString:@MAW_MAP_CENTER_ZOOM_LEVEL])
	{
		centerZoomLevel = [value intValue];
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED])
	{
		if ([value isEqualToString:@"true"])
		{
			[self setCenterCoordinate:centerCoordinates zoomLevel:centerZoomLevel animated:YES];
		}

	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE])
	{
		upperLeftPointCoordinate.latitude = [value doubleValue];
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE])
	{
		upperLeftPointCoordinate.longitude = [value doubleValue];
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE])
	{
		lowerRightPointCoordinate.latitude = [value doubleValue];
	}
	else if ([key isEqualToString:@MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE])
	{
		lowerRightPointCoordinate.longitude = [value doubleValue];
	}
	else if ([key isEqualToString:@MAW_MAP_CENTERED_ON_VISIBLE_AREA])
	{
		MKMapPoint nwPoint = MKMapPointForCoordinate(upperLeftPointCoordinate);
		MKMapPoint sePoint = MKMapPointForCoordinate(lowerRightPointCoordinate);
		MKMapRect nwRect = MKMapRectMake(nwPoint.x, nwPoint.y, 0, 0);
		MKMapRect seRect = MKMapRectMake(sePoint.x, sePoint.y, 0, 0);
		MKMapRect zoomRect = MKMapRectUnion(nwRect, seRect);

		[mapView setVisibleMapRect:zoomRect animated:TRUE];
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
		if (mapView.mapType == MKMapTypeStandard)
		{
			return [[NSString alloc] initWithFormat:@"%d", MAW_MAP_TYPE_ROAD];
		}
		else
		{
			return [[NSString alloc] initWithFormat:@"%d", MAW_MAP_TYPE_SATELLITE];
		}

	}
	else if ([key isEqualToString:@MAW_MAP_ZOOM_LEVEL])
	{
		return [[NSString alloc] initWithFormat:@"%d", [self getMapViewZoomLevel]];
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

#pragma mark -
#pragma mark Map conversion methods

- (double)longitudeToPixelSpaceX:(double)longitude
{
    return round(MERCATOR_OFFSET + MERCATOR_RADIUS * longitude * M_PI / 180.0);
}

- (double)latitudeToPixelSpaceY:(double)latitude
{
    return round(MERCATOR_OFFSET - MERCATOR_RADIUS * logf((1 + sinf(latitude * M_PI / 180.0)) / (1 - sinf(latitude * M_PI / 180.0))) / 2.0);
}

- (double)pixelSpaceXToLongitude:(double)pixelX
{
    return ((round(pixelX) - MERCATOR_OFFSET) / MERCATOR_RADIUS) * 180.0 / M_PI;
}

- (double)pixelSpaceYToLatitude:(double)pixelY
{
    return (M_PI / 2.0 - 2.0 * atan(exp((round(pixelY) - MERCATOR_OFFSET) / MERCATOR_RADIUS))) * 180.0 / M_PI;
}

#pragma mark -
#pragma mark Helper methods

- (MKCoordinateSpan)coordinateSpanWithMapView:(MKMapView *)mapView
							 centerCoordinate:(CLLocationCoordinate2D)centerCoordinate
								 andZoomLevel:(NSUInteger)zoomLevel
{
    // convert center coordiate to pixel space
    double centerPixelX = [self longitudeToPixelSpaceX:centerCoordinate.longitude];
    double centerPixelY = [self latitudeToPixelSpaceY:centerCoordinate.latitude];

    // determine the scale value from the zoom level
    NSInteger zoomExponent = 20 - zoomLevel;
    double zoomScale = pow(2, zoomExponent);

    // scale the map’s size in pixel space
    CGSize mapSizeInPixels = mapView.bounds.size;
    double scaledMapWidth = mapSizeInPixels.width * zoomScale;
    double scaledMapHeight = mapSizeInPixels.height * zoomScale;

    // figure out the position of the top-left pixel
    double topLeftPixelX = centerPixelX - (scaledMapWidth / 2);
    double topLeftPixelY = centerPixelY - (scaledMapHeight / 2);

    // find delta between left and right longitudes
    CLLocationDegrees minLng = [self pixelSpaceXToLongitude:topLeftPixelX];
    CLLocationDegrees maxLng = [self pixelSpaceXToLongitude:topLeftPixelX + scaledMapWidth];
    CLLocationDegrees longitudeDelta = maxLng - minLng;

    // find delta between top and bottom latitudes
    CLLocationDegrees minLat = [self pixelSpaceYToLatitude:topLeftPixelY];
    CLLocationDegrees maxLat = [self pixelSpaceYToLatitude:topLeftPixelY + scaledMapHeight];
    CLLocationDegrees latitudeDelta = -1 * (maxLat - minLat);

    // create and return the lat/lng span
    MKCoordinateSpan span = MKCoordinateSpanMake(latitudeDelta, longitudeDelta);
    return span;
}

- (void)setCenterCoordinate:(CLLocationCoordinate2D)centerCoordinate
				  zoomLevel:(NSUInteger)zoomLevel
				   animated:(BOOL)animated
{
	MKMapView* mapView = (MKMapView*) view;

    // clamp large numbers to 28
	if (zoomLevel < 1)
	{
		zoomLevel = 1;
	}
	else if (zoomLevel > 21)
	{
		zoomLevel = 21;
	}

    // use the zoom level to compute the region
    MKCoordinateSpan span = [self coordinateSpanWithMapView:mapView centerCoordinate:centerCoordinate andZoomLevel:zoomLevel];
    MKCoordinateRegion region = MKCoordinateRegionMake(centerCoordinate, span);

    // set the region like normal
    [mapView setRegion:region animated:animated];
}

- (int)getMapViewZoomLevel
{
	// TODO implement the zoom level
	return 0;
}

@end
