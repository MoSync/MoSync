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
 * @file MapListener.h
 * @author Spiridon Alexandru
 *
 * \brief Listener for Map events.
 */

#include "MapPin.h"

#ifndef NATIVEUI_MAP_LISTENER_H_
#define NATIVEUI_MAP_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Map;

    /**
     * \brief Listener for Map events.
     */
    class MapListener
    {
    public:
        /**
		 * This method is called when the zoom level of the map changes (at a double tap
		 * on the google maps for example).
         * @param map The map object that generated the event.
         */
        virtual void mapZoomLevelChanged(Map* map) {};

        /**
         * This method is called when the visible region on the map is changed (on a drag/scroll
		 * for example).
         * @param map The map object that generated the event.
         */
        virtual void mapRegionChanged(Map* map) {};

		/**
		 * This method is called when the user clicks on a map pin.
		 * @param mapPin The map pin object that generated the event.
		 */
		virtual void mapPinClicked(MapPin* mapPin) {};
    };

} // namespace NativeUI

#endif /* NATIVEUI_MAP_LISTENER_H_ */

/*! @} */
