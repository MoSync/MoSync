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

#import <Foundation/Foundation.h>
#import <MapKit/MKMapView.h>
#import <MapKit/MapKit.h>
#import "IWidget.h"

/**
 * Displays a map to the user.
 */
@interface MapWidget: IWidget<MKMapViewDelegate> {
	/**
	 * Contains the current zoom level of the map.
	 */
	int currentMapZoomLevel;

	/**
	 * Contains the center zoom level.
	 */
	int centerZoomLevel;

	/**
	 * Contains the center coordinates of the map.
	 */
	CLLocationCoordinate2D centerCoordinates;

	/**
	 * Contains the upper left corner coordinates of the visible area.
	 */
	CLLocationCoordinate2D upperLeftPointCoordinate;

	/**
	 * Contains the lower right corner coordinates of the visible area.
	 */
	CLLocationCoordinate2D lowerRightPointCoordinate;
}

/**
 * Init function.
 */
- (id)init;

/**
 * Adds a child to the map.
 * @param child Widget to be added. Must be an MapPinWidget.
 * @return One of the fallowing result codes:
 * - MAW_RES_OK for success
 * - MAW_RES_INVALID_LAYOUT if child's type is not MapPinWidget.
 */
- (int)addChild:(IWidget*)child;

/**
 * Removes a annotation from the map.
 */
- (int)remove;

/**
 * Removes a child from the map.
 */
- (void)removeChild: (IWidget*)child;

/**
 * Sets a map property.
 * @param key The property of the map widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

/**
 * Returns a property value of the map widget.
 * @param key The property of the map widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key;

@end
