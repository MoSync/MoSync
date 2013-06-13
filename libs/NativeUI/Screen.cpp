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

/**
 * @file Screen.cpp
 * @author Mikael Kindborg
 *
 * Class that represents a visible screen. Only one screen
 * is visible at a time.
 */

#include "Screen.h"
#include "ScreenListener.h"
#include "NavigationBarButton.h"

#define BUF_MAX 256

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	Screen::Screen() :
		Widget(MAW_SCREEN)
	{
	}

	/**
	 * Protected because only subclasses should use this constructor.
	 * @widgetType The string constant that identifies the widget type
	 * (one of the MAW_ constants).
	 */
	Screen::Screen(const MAUtil::String& widgetType) :
		Widget(widgetType)
	{
	}

	/**
	 * Destructor.
	 */
	Screen::~Screen()
	{
		mScreenListeners.clear();
		mActionBarItems.clear();
	}

	/**
	 * Set the title of the screen.
	 * The title is used by tab screen to display a text on the tab indicator.
	 * @param title The screen title.
	 */
	void Screen::setTitle(const MAUtil::String& title)
	{
		setProperty(MAW_SCREEN_TITLE, title.c_str());
	}

	/**
	 * Set the icon of the screen. The icon is displayed
	 * on tab screens.
	 * @param imageHandle Handle to an image with the icon.
	 * @return The result code.
	 */
	int Screen::setIcon(const MAHandle imageHandle)
	{
		return setPropertyInt(MAW_SCREEN_ICON, imageHandle);
	}

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
	int Screen::setMainWidget(Widget* widget)
	{
	    int nrChildren = this->countChildWidgets();
	    for (int i = 0; i < nrChildren; i++)
	    {
	        this->removeChild(this->getChild(i));
	    }

	    return addChild(widget);
	}

	/**
	 * Show a screen. Only one screen at a time is visible.
	 * The previous screen will be hidden when showing a screen.
	 * Note: This method is only applicable to screens.
	 */
	void Screen::show()
	{
		maWidgetScreenShow(getWidgetHandle());
	}

	/**
	 * Show a screen with transition. Only one screen at a time is visible.
	 * The previous screen will be hidden when showing a screen.
	 * Note: This method is only applicable to screens.
	 *
	 * @param screenTransitionType The type of the screen transition. See available
	 * screen transitions types \link #MA_TRANSITION_TYPE_NONE here \endlink.
	 * @param screenTransitionDuration The duration of the screen transition in
	 * milliseconds.
	 * This argument is not used on the Windows Phone platform due to the constant
	 * duration of
	 * the WP screen transitions.
	 *
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the show with transition operation was successful.
	 * - #MAW_RES_INVALID_SCREEN_TRANSITION_TYPE if the screen transition type is not
	 * available on current platform. Show operation is still completed without screen
	 * transition.
	 * - #MAW_RES_INVALID_SCREEN_TRANSITION_DURATION if the screen transition is not a
	 * positive integer. This error code is not returned on the Windows Phone platform
	 * due to the constant duration of the WP screen transitions.
	 */
	int Screen::showWithTransition(MAWScreenTransitionType screenTransitionType,
		int screenTransitionDuration)
	{
		return maWidgetScreenShowWithTransition(getWidgetHandle(), screenTransitionType,
			screenTransitionDuration);
	}

	/**
	 * @deprecated: Use addScreenListener(ScreenListener*) instead.
	 * Called just before the screen begins rotating.
	 * Subclasses may override this method to perform additional actions
	 * immediately prior to the rotation.
	 * Note: available only on iOS.
	 */
	void Screen::orientationWillChange()
	{
		// No implementation required.
	}

	/**
	 * @deprecated: Use addScreenListener(ScreenListener*) instead.
	 * Called after the screen has finished rotating.
	 * Subclasses may override this method to perform additional actions
	 * after the rotation.
	 */
	 void Screen::orientationDidChange()
	 {
		// No implementation required
	 }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void Screen::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        if (widgetEventData->eventType == MAW_EVENT_SCREEN_ORIENTATION_WILL_CHANGE)
        {
            this->orientationWillChange();
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->orientationWillChange(this);
			}
        }
		else if (widgetEventData->eventType == MAW_EVENT_SCREEN_ORIENTATION_DID_CHANGE)
		{
			this->orientationDidChange();
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->orientationChanged(
						this, widgetEventData->screenOrientation);
			}
		}
		else if ( MAW_EVENT_OPTIONS_MENU_ITEM_SELECTED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->optionsMenuItemSelected(
						this, widgetEventData->optionsMenuItem);
			}
		}
		else if( MAW_EVENT_OPTIONS_MENU_CLOSED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->optionsMenuClosed(this);
			}
		}
		else if( MAW_EVENT_ACTION_BAR_MENU_ITEM_SELECTED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->actionBarItemSelected(
						this,
						widgetEventData->actionBarMenuItem);
			}
		}
		else if( MAW_EVENT_ACTION_BAR_UP_ITEM_SELECTED == widgetEventData->eventType )
		{
			for (int i=0; i < mScreenListeners.size(); i++)
			{
				mScreenListeners[i]->actionBarUpSelected(this);
			}
		}
    }

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
	int Screen::addOptionsMenuItem(const MAUtil::String title)
	{
		return maWidgetScreenAddOptionsMenuItem(
				getWidgetHandle(), title.c_str(), "", 0 );
	}

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
	* @param resourceIconID The resource id of an icon loaded into resources.
	*
	* @return The index on which the menu item was added in the options menu,
	* an error code otherwise.
	*/
	int Screen::addOptionsMenuItem(const MAUtil::String title, int resourceIconID)
	{
		char buf[BUF_MAX];
		sprintf(buf, "%d", resourceIconID);
		return maWidgetScreenAddOptionsMenuItem(
				getWidgetHandle(), title.c_str(), buf, 0 );
	}

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
			const MAUtil::String title, const MAUtil::String icon, bool isPath)
	{
		if(isPath)
		{
			char platform[BUF_MAX];
			maGetSystemProperty("mosync.device.OS", platform, BUF_MAX);

			if(strcmp(platform, "Android") == 0)
				return MAW_RES_ERROR;
			else
			{
				return maWidgetScreenAddOptionsMenuItem(
				getWidgetHandle(), title.c_str(), icon.c_str(), 0 );
			}
		}
		else return maWidgetScreenAddOptionsMenuItem(
				getWidgetHandle(), title.c_str(), icon.c_str(), 1 );
	}

	/**
	 * Remove the options menu from this screen.
	 * @return True if success, false otherwise ( for instance the options
	 * menu has no content.
	 */
	void Screen::removeOptionsMenu()
	{
		setProperty(MAW_SCREEN_REMOVE_OPTIONS_MENU,"");
	}

    int Screen::addNavBarButton(const NavBarSide side, NavigationBarButton* button)
    {
        return maWidgetScreenInsertNavBarButton(
                                                side,
                                                getWidgetHandle(),
                                                button->getWidgetHandle(),
                                                -1);
    }

    int Screen::insertNavBarButton(const NavBarSide side, NavigationBarButton* button, int index)
    {
        return maWidgetScreenInsertNavBarButton(
                                                side,
                                                getWidgetHandle(),
                                                button->getWidgetHandle(),
                                                index);
    }

    void Screen::removeNavBarButton(NavigationBarButton* button)
    {
        setPropertyInt(MAW_SCREEN_REMOVE_NAV_BAR_BUTTON_WIDGET, button->getWidgetHandle());
    }

    void Screen::removeNavBarButtons(const NavBarSide side)
    {
        setPropertyInt(MAW_SCREEN_REMOVE_NAV_BAR_BUTTONS, side);
    }

    void Screen::replaceBackButton(NavigationBarButton* backButton)
    {
        setPropertyInt(MAW_SCREEN_SET_BACK_BUTTON, backButton->getWidgetHandle());
    }

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
	// MAHandle Screen::addActionBarMenuItem(const MAUtil::String itemTitle, const int iconPredefinedId,
	// 		const MAHandle iconHandle, const ActionBarMenuItemDisplayFlag flag)
	// {
	// 	MA_ACTION_BAR_ITEM_ICON iconArgs;
	// 	iconArgs.iconPredefinedId = iconPredefinedId;
	// 	iconArgs.iconHandle = iconHandle;
	// 	return maActionBarAddMenuItem(getWidgetHandle(), itemTitle.c_str(), &iconArgs, flag);
	// }

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
	// int Screen::removeActionBarMenuItem(MAHandle itemHandle)
	// {
	// 	return maActionBarRemoveMenuItem(getWidgetHandle(), itemHandle);
	// }

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
	// int Screen::removeActionBarItems()
	// {
	// 	int result = MAW_RES_OK;
	// 	for (int i=0 ; i<mActionBarItems.size(); i++)
	// 	{
	// 		result = maActionBarRemoveMenuItem(getWidgetHandle(), mActionBarItems[i]);
	// 	}
	// 	return result;
	// }

	/**
	 * Check if a screen is shown.
	 * @return true if the screen is visible, false otherwise.
	 */
	bool Screen::isShown()
	{
        MAUtil::String value = this->getPropertyString(MAW_SCREEN_IS_SHOWN);
        return (strcmp(value.c_str(), "true") == 0) ? true : false;
	}

    /**
     * Add a screen event listener.
     * @param listener The listener that will receive screen events.
     */
    void Screen::addScreenListener(ScreenListener* listener)
    {
		addListenerToVector(mScreenListeners, listener);
    }

    /**
     * Remove the screen event listener.
     * @param listener The listener that receives screen events.
     */
    void Screen::removeScreenListener(ScreenListener* listener)
    {
		removeListenerFromVector(mScreenListeners, listener);
    }

} // namespace NativeUI
