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
 * @file Location.h
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a point with two coordinates: latitude and longitude.
 */

#ifndef NATIVEUI_LOCATION_H_
#define NATIVEUI_LOCATION_H_

namespace NativeUI
{
	class Location
	{
	public:
		/**
		 * Constructor
		 */
		Location();

		/**
		 * Constructor: creates a Location object based on a given latitude and longitude.
		 * @param latitude The latitude of the point.
		 * @param longitude The longitde of the point
		 */
		Location(double latitude, double longitude);

		/**
         * Destructor.
         */
        ~Location();

		/**
		 * Setter for the latitude member.
		 * @param latitude The new latitude.
		 */
		void setLatitude(double latitude);

		/**
		 * Getter for the latitude member.
		 */
		double getLatitude() const;

		/**
		 * Setter for the longitude member.
		 * @param longitude The new longitude.
		 */
		void setLongitude(double longitude);

		/**
		 * Getter for the longitude member.
		 */
		double getLongitude() const;

		/**
		 * Assignment operator.
		 */
		Location & operator=(const Location &rightHandSide);
	protected:
		/**
		 * The latitude coordinate for the current location.
		 */
		double mLatitude;

		/**
		 * The longitude coordinate for the current location.
		 */
		double mLongitude;
	};
} // end of NativeUI namespace

#endif /* NATIVEUI_LOCATION_H_ */

/*! @} */
