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
