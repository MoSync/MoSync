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
 * @file ScreenListener.h
 * @author emma
 *
 * \brief Listener for Screen events.
 *
 */

#ifndef NATIVEUI_SCREEN_LISTENER_H_
#define NATIVEUI_SCREEN_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Screen;

    /**
     * \brief Interface for Screen event listeners.
     * Use it when the screen has attached an options menu.
     * The options menu can be set to a screen by adding items
     * through addOptionsMenuItem.
     */
    class ScreenListener
    {
    public:
		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 * @param screenOrientation The new screen orientation.
		 */
		virtual void orientationChanged(Screen* screen, int screenOrientation) {};

		/**
		 * Called just before the screen begins rotating.
		 * Subclasses may override this method to perform additional actions
		 * immediately prior to the rotation.
		 * Note: available only on iOS.
		 */
		virtual void orientationWillChange(Screen* screen) {};

        /**
         * This method is called when the OptionsMenu is being closed
         * (either by the user canceling the menu with the back/menu
         * button, or when an item is selected.
         * @param screen The screen that generated the event.
         */
        virtual void optionsMenuClosed(Screen* screen) {};

        /**
         * This method is called when an options menu item is selected.
         * @param index The index on which the item is placed into the
         * OptionsMenu.
         * @param screen The screen that generated the event.
         */
        virtual void optionsMenuItemSelected(Screen* screen, int index) {};

        /**
         * This method is called when a menu item from the Action Bar is selected.
         * @param screen The screen that generated the event.
         * @param itemHandle The handle of the menu item that was selected.
         */
        virtual void actionBarItemSelected(Screen* screen, int itemHandle) {};

        /**
         * This method is called when the Up indicator from the Action Bar is selected.
         * @param screen The screen that generated the event.
         */
        virtual void actionBarUpSelected(Screen* screen){};
    };

} // namespace NativeUI

#endif /* NATIVEUI_SCREEN_LISTENER_H_ */

/*! @} */
