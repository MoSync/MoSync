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

#ifndef NATIVEUI_MAP_PIN_LISTENER_H_
#define NATIVEUI_MAP_PIN_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class MapPin;

    /**
     * \brief Listener for Map pin events.
     */
    class MapPinListener
    {
    public:
        /**
		 * This method is called when the user clicks on a map pin.
         * @param mapPin The map pin object that generated the event.
         */
        virtual void mapPinClicked(MapPin* mapPin) {};
    };

} // namespace NativeUI

#endif /* NATIVEUI_MAP_PIN_LISTENER_H_ */

/*! @} */
