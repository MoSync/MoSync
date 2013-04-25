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
 * Util.h
 *
 *  Created on: Aug 7, 2012
 *      Author: Spiridon Alexandru
 */

#include <mastring.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <NativeUI/ListViewItem.h>

#ifndef UTIL_H_
#define UTIL_H_

#define TITLE_FONT_SIZE 26.0

/**
 * Detects if the current platform is Android.
 * @return true if the platform is Android, false otherwise.
 */
bool isAndroid();

/**
 * Detects if the current platform is iOS.
 * @return true if the platform is iOS, false otherwise.
 */
bool isIOS();

/**
 * Detects if the current platform is Windows Phone.
 * @return true if the platform is Windows Phone, false otherwise.
 */
bool isWindowsPhone();


/**
 * Get the string associated with a ListViewItemSelectionStyle.
 * @param style The given style.
 * @return The string associated with the given style.
 */
MAUtil::String getSelectionStyleString(NativeUI::ListViewItemSelectionStyle style);

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

#endif /* UTIL_H_ */
