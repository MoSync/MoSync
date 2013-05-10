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
