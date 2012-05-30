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
