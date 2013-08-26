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
 * Util.h
 *
 *  Created on: Sept 28, 2012
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
