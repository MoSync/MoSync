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
