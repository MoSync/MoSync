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

/**
 * @file MapLocation.cpp
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a point on the map (it has two coordinates: latitude and longitude and a zoom level).
 */

#include "MapLocation.h"

namespace NativeUI
{
	/**
	 * Constructor
	 */
	MapLocation::MapLocation()
	{
		mLatitude = 0.0;
		mLongitude = 0.0;
		mZoomLevel = 0;
	}

	/**
	 * Constructor: creates a MapLocation object based on a given latitude and longitude.
	 * @param latitude The latitude of the point.
	 * @param longitude The longitde of the point.
	 */
	MapLocation::MapLocation(double latitude, double longitude) : Location(latitude, longitude)
	{
		mZoomLevel = 0;
	}

	/**
	 * Constructor: creates a MapLocation object base on a given latitude, longitude and zoom level.
	 * @param latitude The latitude of the point.
	 * @param longitude The longitde of the point.
	 * @param zoomLevel The zoom level at wich the map needs to be positioned.
	 */
	MapLocation::MapLocation(double latitude, double longitude, int zoomLevel) : Location(latitude, longitude)
	{
		mZoomLevel = zoomLevel;
	}

	/**
	 * Destructor.
	 */
	MapLocation::~MapLocation()
	{
	}

	/**
	 * Setter for the zoom level of the current map point.
	 * @param zoomLevel The new zoom level.
	 */
	void MapLocation::setZoomLevel(int zoomLevel)
	{
		mZoomLevel = zoomLevel;
	}

	/**
	 * Getter for the zoom level of the current map point.
	 */
	int MapLocation::getZoomLevel() const
	{
		return mZoomLevel;
	}

	/**
	 * Setter for the location member.
	 * @param location The new location for the map point.
	 */
	void MapLocation::setLocation(Location* location)
	{
		mLatitude = location->getLatitude();
		mLongitude = location->getLongitude();
	}

	/**
	 * Assignment operator.
	 */
	MapLocation & MapLocation::operator=(const MapLocation &rightHandSide)
	{
		mLatitude = rightHandSide.getLatitude();
		mLongitude = rightHandSide.getLongitude();
		mZoomLevel = rightHandSide.getZoomLevel();

		return *this;
	}
} // end of NativeUI namespace
