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
 * @file WidgetUtil.h
 * @author Bogdan Iusco
 *
 * Utilities for widgets(e.g. common constants, macros, functions etc).
 */

#ifndef WIDGET_UTIL_H_
#define WIDGET_UTIL_H_

#include <MAUtil/Vector.h>


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

#endif /* WIDGET_UTIL_H_ */
