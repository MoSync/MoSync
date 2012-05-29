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
	MapRegion::MapRegion(Location* upperLeftCorner, Location* lowerRightCorner)
	{
		mUpperLeftCorner = new Location();
		mLowerRightCorner = new Location();
		mUpperLeftCorner = upperLeftCorner;
		mLowerRightCorner = lowerRightCorner;
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
	Location& MapRegion::getUpperLeftCorner()
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
	Location& MapRegion::getLowerRightCorner()
	{
		return *mLowerRightCorner;
	}
} // end of NativeUI namespace
