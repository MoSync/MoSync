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
 * @file MapRegion.cpp
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a region on the map (it has two coordinates: the upper left corner and the lower right corner).
 */

#include "MapRegion.h"

namespace NativeUI
{
	/**
	 * Constructor
	 */
	MapRegion::MapRegion()
	{
		mUpperLeftCorner = new Location();
		mLowerRightCorner = new Location();
	}

	/**
	 * Constructor: creates a MapRegion object based on a given upper left corner and lower right corner.
	 * @param upperLeftCorner The upper left corner of the map region.
	 * @param lowerRightCorner The lower right corner of the map region.
	 */
	MapRegion::MapRegion(Location upperLeftCorner, Location lowerRightCorner)
	{
		mUpperLeftCorner = new Location();
		mLowerRightCorner = new Location();
		*mUpperLeftCorner = upperLeftCorner;
		*mLowerRightCorner = lowerRightCorner;
	}

	/**
	 * Destructor.
	 */
	MapRegion::~MapRegion()
	{
		delete mUpperLeftCorner;
		delete mLowerRightCorner;
	}

	/**
	 * Setter for the upper left corner of the current map region.
	 * @param upperLeftCorner The new upper left corner.
	 */
	void MapRegion::setUpperLeftCorner(Location* upperLeftCorner)
	{
		mUpperLeftCorner = upperLeftCorner;
	}

	/**
	 * Getter for the upper left corner of the current map region.
	 */
	Location& MapRegion::getUpperLeftCorner() const
	{
		return *mUpperLeftCorner;
	}

	/**
	 * Setter for the lower right corner of the current map region.
	 * @param lowerRightCorner The new lower right corner.
	 */
	void MapRegion::setLowerRightCorner(Location* lowerRightCorner)
	{
		mLowerRightCorner = lowerRightCorner;
	}

	/**
	 * Getter for the lower right corner of the current map region.
	 */
	Location& MapRegion::getLowerRightCorner() const
	{
		return *mLowerRightCorner;
	}

		/**
	 * Assignment operator.
	 */
	MapRegion & MapRegion::operator=(const MapRegion &rightHandSide)
	{
		(*mUpperLeftCorner) = rightHandSide.getUpperLeftCorner();
		(*mLowerRightCorner) = rightHandSide.getLowerRightCorner();

		return *this;
	}
} // end of NativeUI namespace
