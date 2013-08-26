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

	/**
	 * \brief Constants indicating how a menu item should display in
	 * the presence of an Action Bar.
	 * Platform: Android only.
	 */
	enum ActionBarMenuItemDisplayFlag
	{
		/**
		 * @brief Show this item as a button in an Action Bar if the system
		 * decides there is room for it.
		 */
		SHOW_IF_ROOM = 1,
		/**
		 * @brief Always show this item as a button in an Action Bar.
		 * Use sparingly! If too many items are set to always show in the Action Bar it can
		 * crowd it and degrade the user experience on devices with smaller screens.
		 * A good rule of thumb is to have no more than 2 items set to always show at a time.
		 */
		SHOW_ALWAYS = 2,
		/**
		 * @brief When this item is in the action bar, always show it with a text label even
		 * if it also has an icon specified.
		 */
		SHOW_WITH_TEXT = 4,
		/**
		 * @brief This item's action view collapses to a normal menu item.
		 * When expanded, the action view temporarily takes over a larger segment of its container.
		 */
		SHOW_COLLAPSE_ACTION_VIEW = 8
	};

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
		 * Show a screen with transition. Only one screen at a time is visible.
		 * The previous screen will be hidden when showing a screen.
		 * Note: This method is only applicable to screens.
		 *
		 * @param screenTransitionType The type of the screen transition. See available
		 * screen transitions types \link #MAW_TRANSITION_TYPE_NONE here \endlink.
		 * @param screenTransitionDuration The duration of the screen transition in milliseconds.
		 * This argument is not used on the Windows Phone platform due to the constant duration of
		 * the WP screen transitions.
		 *
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the show with transition operation was successful.
		 * - #MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not available
		 * on current platform. Show operation is still completed without screen transition.
		 * - #MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the screen transition is not a positive
		 * integer. This error code is not returned on the Windows Phone platform
		 * due to the constant duration of the WP screen transitions.
		 */
		virtual int showWithTransition(MAWScreenTransitionType screenTransitionType, int screenTransitionDuration);

		/**
		 * @deprecated: Use addScreenListener(ScreenListener*) instead.
		 * Called just before the screen begins rotating.
		 * Subclasses may override this method to perform additional actions
		 * immediately prior to the rotation.
		 * Note: available only on iOS.
		 */
		virtual void orientationWillChange();

		/**
		 * @deprecated: Use addScreenListener(ScreenListener*) instead.
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
		int addOptionsMenuItem(const MAUtil::String title);

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
		int addOptionsMenuItem(const MAUtil::String title, int resourceIconID);

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
		int addOptionsMenuItem(
				const MAUtil::String title, const MAUtil::String icon, bool isPath);

		/**
		 * Remove the options menu from this screen.
		 */
		virtual void removeOptionsMenu();

		/**
		 * Add a menu item to the Action bar, with a given title
		 * and an icon using either a predefined system image, either
		 * a resource image.
		 * NOTE that before making any Action bar specific call,
		 * it must be enabled using: ActionBar::setEnabled(true);
		 *
		 * Remember to call ActionBar::getInstance()->refresh()
		 * when you finished with menu items changes.
		 *
		 * @param itemTitle The title associated for the new item. Can be left null.
		 * @param iconPredefinedId The icon for this item as a Drawable, getting it from
		 * predefined system resources. One of the #OptionsMenuIconConstants.
		 * Can be left to -1 if the item uses a project resource instead.
		 * @param iconHandle The icon for this item as a Drawable, getting it from resources.
		 * Can be left to -1 if the item uses a predefined system resource instead.
		 *
		 * @return The menu item handle on success, or any of the following result codes:
		 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform,
		 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar is not enabled, or
		 * -
		 */
		// MAHandle addActionBarMenuItem(const MAUtil::String itemTitle, const int iconPredefinedId,
		// 		const MAHandle iconHandle, const ActionBarMenuItemDisplayFlag flag);

		/**
		 * Remove a menu item from the Action bar.
		 * NOTE that before making any Action bar specific call,
		 * it must be enabled using: ActionBar::setEnabled(true);
		 *
		 * Remember to call ActionBar::getInstance()->refresh()
		 * when you finished with menu items changes.
		 *
		 * @returns #MAW_RES_OK on success, or any of the following result codes:
		 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar is not available on the current platform,
		 * - #MAW_RES_ACTION_BAR_DISABLED If the action bar is not enabled, or
		 * - #MAW_RES_INVALID_HANDLE If the indicated screen has no item on specified handle.
		 */
		// int removeActionBarMenuItem(MAHandle itemHandle);

		/**
		 * Remove all Action bar menu items from the current screen.
		 * NOTE that before making any Action bar specific call,
		 * it must be enabled using: ActionBar::setEnabled(true);
		 *
		 * Remember to call ActionBar::getInstance()->refresh()
		 * when you finished with menu items changes.
		 *
		 * @return #MAW_RES_OK on success, or any of the following result codes:
		 * - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the Action bar is not available on the current platform,
		 * - #MAW_RES_ACTION_BAR_DISABLED If the Action bar is not enabled, or
		 * - #MAW_RES_INVALID_HANDLE If the indicated screen has no menu item on specified handle.
		 */
		// int removeActionBarItems();

		/**
		 * Check if a screen is shown.
		 * @return true if the screen is visible, false otherwise.
		 */
		virtual bool isShown();

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
        MAUtil::Vector<MAHandle> mActionBarItems;
	};

} // namespace NativeUI

#endif

/*! @} */
