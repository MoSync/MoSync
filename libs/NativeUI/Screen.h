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
 * @file Screen.h
 * @author Mikael Kindborg
 *
 * \brief Class that represents a visible screen. Only one screen
 * is visible at a time.
 * For screen events see ScreenListener.
 */

#ifndef NATIVEUI_SCREEN_H_
#define NATIVEUI_SCREEN_H_

#include "Widget.h"

namespace NativeUI
{

	// Forward declaration.
	class ScreenListener;

	/**
	 * \brief Class that represents a visible screen.
	 * For screen events see ScreenListener.
	 */
	class Screen : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		Screen();

		/**
		 * Destructor.
		 */
		virtual ~Screen();

		/**
		 * Set the title of the screen.
		 * The title is used by tab screen to display a text on the
		 * tab indicator.
		 * @param title The screen title.
		 */
		void setTitle(const MAUtil::String& title);

		/**
		 * Set the icon of the screen. The icon is displayed
		 * on tab screens.
		 * Note: this is not available on Windows Phone 7.
		 * @param imageHandle Handle to an image with the icon.
		 * @return The result code.
		 */
		int setIcon(const MAHandle imageHandle);

		/**
		 * Set the main widget of the screen.
		 * Note: A screen can only have one main widget.
		 * Use a layout as main widget and add child
		 * widgets to the layout.
		 * @param widget The widget to be set as main widget.
		 * The ownership of the widget is passed to this function.
		 * When the screen will be destroyed, the widget and it's child widgets
		 * will be deleted.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be added to the parent.
		 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
		 * - #MAW_RES_ERROR if it could not be added for some other reason.
		 */
		int setMainWidget(Widget* widget);

		/**
		 * Show a screen. Only one screen at a time is visible.
		 * The previous screen will be hidden when showing a screen.
		 * Note: This method is only applicable to screens.
		 */
		virtual void show();

		/**
		 * Called just before the screen begins rotating.
		 * Subclasses may override this method to perform additional actions
		 * immediately prior to the rotation.
		 */
		virtual void orientationWillChange();

		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 */
		virtual void orientationDidChange();

		/**
		* Add a new menu item to the Options Menu associated to this screen.
		* Platform: Android and WP7.
		* Option Menus are Android specific concept. The Options Menu is launched by
		* pressing the Menu key. The options menu is where you should include
		* actions and other options that are relevant to the current activity
		* context, such as "Search," "Compose email," or "Settings".
		* When opened, the first visible portion is the icon menu, which holds
		* up to six menu items. If your menu includes more than six items, Android
		* places the sixth item and the rest into the overflow menu, which the user
		* can open by selecting More. Those items do not display icons. On Windows
		* Phone 7 the control used is the application bar.
		*
		* @param title The title associated for the new item.
		*
		* Note: On Windows phone 7, by using this function you will obtain an
		* application bar menu item (text only)
		*
		* @return The index on which the menu item was added in the options menu,
		* an error code otherwise.
		*/
		int Screen::addOptionsMenuItem(const MAUtil::String title);

		/**
		* Add a new menu item to the Options Menu associated to this screen.
		* Platform: Android.
		* Option Menus are Android specific concept. The Options Menu is launched by
		* pressing the Menu key. The options menu is where you should include
		* actions and other options that are relevant to the current activity
		* context, such as "Search," "Compose email," or "Settings".
		* When opened, the first visible portion is the icon menu, which holds
		* up to six menu items. If your menu includes more than six items, Android
		* places the sixth item and the rest into the overflow menu, which the user
		* can open by selecting More. Those items do not display icons. On Windows
		* Phone 7 the control used is the application bar.
		*
		* @param title The title associated for the new item. Can be left null.
		* @param resourceIconID The resource id of an icon loaded into resources
		*
		* @return The index on which the menu item was added in the options menu,
		* an error code otherwise.
		*/
		int Screen::addOptionsMenuItem(const MAUtil::String title, int resourceIconID);

		/**
		* Add a new menu item to the Options Menu associated to this screen.
		* Platform: Android and WP7.
		* Option Menus are Android specific concept. The Options Menu is launched by
		* pressing the Menu key. The options menu is where you should include
		* actions and other options that are relevant to the current activity
		* context, such as "Search," "Compose email," or "Settings".
		* When opened, the first visible portion is the icon menu, which holds
		* up to six menu items. If your menu includes more than six items, Android
		* places the sixth item and the rest into the overflow menu, which the user
		* can open by selecting More. Those items do not display icons. On Windows
		* Phone 7 the control used is the application bar.
		*
		* @param title The title associated for the new item. Can be left null.
		* @param icon The id of an icon from the Android and WP7 predefined icon set,
		* these can be found under the OptionsMenuIconConstants group, or the name of
		* the icon file (works only for WP7).
		* @param isPath If true, indicates that the icon is based on the path, otherwise
		* it is one of the predefined icon constants.
		*
		* Note: For Windows phone 7 the option menu icons must be added under the following folder
		* structure "/ApplicationBarIcons/". By using this function you will obtain an
		* application bar icon button (text + icon). Note that only 4 buttons are
		* visible on an application bar. If you exceed this limit the option menu
		* items will be added as application bar menu items on WP7.
		*
		* @return The index on which the menu item was added in the options menu,
		* an error code otherwise.
		*/
		int Screen::addOptionsMenuItem(
				const MAUtil::String title, const MAUtil::String icon, bool isPath);

		/**
		 * Remove the options menu from this screen.
		 */
		virtual void removeOptionsMenu();

        /**
         * Add a screen event listener.
         * @param listener The listener that will receive screen events.
         */
        virtual void addScreenListener(ScreenListener* listener);

        /**
         * Remove the screen event listener.
         * @param listener The listener that receives screen events.
         */
        virtual void removeScreenListener(ScreenListener* listener);
	protected:
		/**
		 * Protected because only subclasses should use this constructor.
		 * @widgetType The string constant that identifies the widget type
		 * (one of the MAW_ constants).
		 */
		Screen(const MAUtil::String& widgetType);

        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);
    private:
        /**
         * Array with screen listeners.
         */
        MAUtil::Vector<ScreenListener*> mScreenListeners;
	};

} // namespace NativeUI

#endif

/*! @} */
