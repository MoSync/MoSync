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
 * @file Location.cpp
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a point with two coordinates: latitude and longitude.
 */

#include "Location.h"

namespace NativeUI
{
	 /**
	 * Constructor
	 */
	Location::Location()
	{
		mLatitude = 0.0;
		mLongitude = 0.0;
	}

	/**
	 * Constructor: creates a Location object based on a given latitude and longitude.
	 * @param latitude The latitude of the point.
	 * @param longitude The longitde of the point
	 */
	Location::Location(double latitude, double longitude)
	{
		mLatitude = latitude;
		mLongitude = longitude;
	}

	/**
	 * Destructor.
	 */
	Location::~Location()
	{
	}

	/**
	 * Setter for the latitude member.
	 * @param latitude The new latitude.
	 */
	void Location::setLatitude(double latitude)
	{
		mLatitude = latitude;
	}

	/**
	 * Getter for the latitude member.
	 */
	double Location::getLatitude() const
	{
		return mLatitude;
	}

	/**
	 * Setter for the longitude member.
	 * @param longitude The new longitude.
	 */
	void Location::setLongitude(double longitude)
	{
		mLongitude = longitude;
	}

	/**
	 * Getter for the longitude member.
	 */
	double Location::getLongitude() const
	{
		return mLongitude;
	}

	/**
	 * Assignment operator.
	 */
	Location & Location::operator=(const Location &rightHandSide)
	{
		mLatitude = rightHandSide.getLatitude();
		mLongitude = rightHandSide.getLongitude();

		return *this;
	}
} // end of NativeUI namespace
