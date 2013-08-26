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
 * @file NavigationBarListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for NavigationBar events.
 */

#ifndef NATIVEUI_NAVIGATION_BAR_LISTENER_H_
#define NATIVEUI_NAVIGATION_BAR_LISTENER_H_

namespace NativeUI
{

    // Forward declaration.
    class NavigationBar;

    /**
     * \brief Listener for NavigationBar events.
     */
    class NavigationBarListener
    {
    public:
        /**
         * This method is called when the back button from the navigation bar
         * was clicked.
         * @param navigationBar The navigation bar object that generated the
         * event.
         */
        virtual void navigationBarBackButtonClicked(
            NavigationBar* navigationBar) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_NAVIGATION_BAR_LISTENER_H_ */

/*! @} */
