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
