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
#import <MapKit/MapKit.h>
#import "IWidget.h"
#import "MapAnnotation.h"

/**
 * Displays a map pin to the user.
 */
@interface MapPinWidget: IWidget {
    /**
     * The coordinates of the annotation.
     */
    CLLocationCoordinate2D annotationCoordinate;

    /**
     * The annotation.
     */
    MapAnnotation *annotation;
}

/**
 * Init function.
 */
- (id)init;

/**
 * Sets a map pin property.
 * @param key The property of the map pin widget that should be set.
 * @param value The value of the property.
 * @return MAW_RES_OK if the property was set, or an error code otherwise.
 */
- (int)setPropertyWithKey: (NSString*)key toValue: (NSString*)value;

/**
 * Returns a property value of the map pin widget.
 * @param key The property of the map pin widget.
 * @return The value for the given property.
 */
- (NSString*)getPropertyWithKey: (NSString*)key;

/**
 * Returns the annotation view that will be added on the map.
 */
- (MapAnnotation*)getAnnotation;

@end
