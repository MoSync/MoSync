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
 * @file MapRegion.h
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a region on the map (it has two coordinates: the upper left corner and the lower right corner).
 */

#ifndef NATIVEUI_MAP_REGION_H_
#define NATIVEUI_MAP_REGION_H_

#include "Location.h"

namespace NativeUI
{
	class MapRegion
	{
	public:
		/**
		 * Constructor
		 */
		MapRegion();

		/**
		 * Constructor: creates a MapRegion object based on a given upper left corner and lower right corner.
		 * @param upperLeftCorner The upper left corner of the map region.
		 * @param lowerRightCorner The lower right corner of the map region.
		 */
		MapRegion(Location upperLeftCorner, Location lowerRightCorner);

		/**
         * Destructor.
         */
        ~MapRegion();

		/**
		 * Setter for the upper left corner of the current map region.
		 * @param upperLeftCorner The new upper left corner.
		 */
		void setUpperLeftCorner(Location* upperLeftCorner);

		/**
		 * Getter for the upper left corner of the current map region.
		 */
		Location& getUpperLeftCorner() const;

		/**
		 * Setter for the lower right corner of the current map region.
		 * @param lowerRightCorner The new lower right corner.
		 */
		void setLowerRightCorner(Location* lowerRightCorner);

		/**
		 * Getter for the lower right corner of the current map region.
		 */
		Location& getLowerRightCorner() const;

		/**
		 * Assignment operator.
		 */
		MapRegion & operator=(const MapRegion &rightHandSide);
	private:
		/**
		 * The upper left corner of the map region.
		 */
		Location* mUpperLeftCorner;

		/**
		 * The lower right corner of the map region.
		 */
		Location* mLowerRightCorner;
	};
} // end of NativeUI namespace

#endif /* NATIVEUI_MAP_REGION_H_ */

/*! @} */
