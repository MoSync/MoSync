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
