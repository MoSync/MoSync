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
