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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file MapLocation.h
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a point on the map (it has two coordinates: latitude and longitude and a zoom level).
 */

#ifndef NATIVEUI_MAP_LOCATION_H_
#define NATIVEUI_MAP_LOCATION_H_

#include "Location.h"

namespace NativeUI
{
	class MapLocation : public Location
	{
	public:
		/**
		 * Constructor
		 */
		MapLocation();

		/**
		 * Constructor: creates a MapLocation object based on a given latitude and longitude.
		 * @param latitude The latitude of the point.
		 * @param longitude The longitde of the point.
		 */
		MapLocation(double latitude, double longitude);

		/**
		 * Constructor: creates a MapLocation object base on a given latitude, longitude and zoom level.
		 * @param latitude The latitude of the point.
		 * @param longitude The longitde of the point.
		 * @param zoomLevel The zoom level at wich the map needs to be positioned.
		 */
		MapLocation(double latitude, double longitude, int zoomLevel);

		/**
         * Destructor.
         */
        ~MapLocation();

		/**
		 * Setter for the zoom level of the current map point.
		 * @param zoomLevel The new zoom level.
		 */
		void setZoomLevel(int zoomLevel);

		/**
		 * Getter for the zoom level of the current map point.
		 */
		int getZoomLevel() const;

		/**
		 * Setter for the location member.
		 * @param location The new location for the map point.
		 */
		void setLocation(Location* location);

		/**
		 * Assignment operator.
		 */
		MapLocation & operator=(const MapLocation &rightHandSide);
	private:
		/**
		 * The zoom level for the current location.
		 */
		int mZoomLevel;
	};
} // end of NativeUI namespace

#endif /* NATIVEUI_MAP_LOCATION_H_ */

/*! @} */
