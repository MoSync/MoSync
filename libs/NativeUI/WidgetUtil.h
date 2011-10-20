/*
Copyright (C) 2011 MoSync AB

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
 * @file WidgetUtil.h
 * @author Bogdan Iusco
 *
 * Utilities for widgets(e.g. common constants, macros, functions etc).
 */

#ifndef WIDGET_UTIL_H_
#define WIDGET_UTIL_H_

#include <MAUtil/Vector.h>

namespace NativeUI
{

    // Usual size for a buffer.
    const int BUF_SIZE = 256;

    // The maximum size for a buffer.
    const int MAX_BUF_SIZE = 2048;

    /**
     * Template method for adding an listener to a vector of listeners.
     * If the vector already contains the listener, it is not added again.
     * So the vector will not contain duplicate listeners.
     * @param vector The vector that will contain the listener.
     * @param listener The given listener.
     */
    template <class Listener>
    void addListenerToVector(
        MAUtil::Vector<Listener*>& vector,
        Listener* listener)
    {
        for (int i = 0; i < vector.size(); i++)
        {
            if (listener == vector[i])
            {
                return;
            }
        }

        vector.add(listener);
    }

    /**
     * Template method for removing an listener from a vector of listeners.
     * Only the first found listener will be removed.
     * @param vector The vector that contains the listener.
     * @param listener The given listener.
     */
    template <class Listener>
    void removeListenerFromVector(
        MAUtil::Vector<Listener*>& vector,
        Listener* listener)
    {
        for (int i = 0; i < vector.size(); i++)
        {
            if (listener == vector[i])
            {
                vector.remove(i);
                break;
            }
        }
    }

} // namespace NativeUI

#endif /* WIDGET_UTIL_H_ */

/*! @} */
