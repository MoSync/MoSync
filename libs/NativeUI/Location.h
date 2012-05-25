/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
