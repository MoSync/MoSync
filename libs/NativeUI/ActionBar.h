/*
Copyright (C) 2013 MoSync AB

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
 * @file ActionBar.h
 * @author Emma Tresanszki
 *
 * \brief The ActionBar singleton class is used for showing
 * and customizing the appearance of the action bar.
 * The Action bar is unique per application, and it will not
 * be treated as a regular widget.
 *
 * NOTE: Available on Android only.
 */


#ifndef ACTIONBAR_H_
#define ACTIONBAR_H_

#include <ma.h>
#include <conprint.h>

#include <MAUtil/String.h>
#include <MAUtil/util.h>

#include <IX_WIDGET.h>
#include "WidgetUtil.h"

namespace NativeUI
{
	/**
	 * \brief Beginning with Android 3.0 (API level 11), the action bar
	 * appears at the top of an activity's window.
	 * By default, the action bar shows the application icon on the left,
	 * followed by the activity title.
	 * The Action bar is unique per application, therefore all syscalls
	 * that relate to the Action Bar will be handled on that unique instance.
	 */
	class ActionBar
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~ActionBar();

		/**
		 * Return the single instance of this class.
		 */
		static ActionBar* getInstance();

		/**
		 * Destroy the single instance of this class.
		 * Call this method only when the application will exit.
		 */
		static void destroyInstance();

		/**
		 * Enable or disable the Action bar.
		 * By default, the Action bar is disabled.
		 * Note that the Action bar is only available starting from Android API level 11.
		 * @param enableActionBar True to enable it, False to disable it.
		 *
		 * @return
		 *  - #MAW_RES_OK on success, or
		 *  - #MAW_RES_ACTION_BAR_NOT_AVAILABLE If the action bar
		 *  is not available on the current platform.
		 */
		bool setEnabled(bool enableActionBar);

		/**
		 * Call this syscall after you finished adding/removing items to the action bar.
		 * Recommended for performance, so that the action bar does not get redrawn each
		 * time one item is changed.
		 */
		void refresh();

		/**
		 * Enable or disable  the display of the Action bar.
		 * se this when you need the application to get into fullscreen, like
		 * for instance in a help screen.
		 * Default value is True if the action bar is enabled on the application.
		 * @param showActionBar True to show the action bar, False to hide it.
		 */
		void show(bool showActionBar);

		/**
		 * Get the current height of the action bar.
		 * @return The height.
		 */
		int getHeight();

		/**
		 * Check if the ActionBar is currently displayed.
		 * @return True if the Action bar is visible, False if it was hidden.
		 */
		bool isShowing();

		/**
		 * Set whether an activity title/subtitle should be displayed.
		 * @param enableTitle True to enable the display of the title, False otherwise.
		 */
		void enableTitle(bool enableTitle);

		/**
		 * Set the action bar's title.
		 * By default, the title is the activity title, in this case application's name.
		 * This will only be displayed if enableTitle(true) is set.
		 * @param title The ActionBar title to set.
		 */
		void setTitle(const MAUtil::String title);

		/**
		 * Set whether to display the activity logo rather than the activity icon.
		 * A logo is often a wider, more detailed image.
		 * @param enableLogo True to enable logo, False otherwise.
		 */
		void enableLogo(bool enableLogo);

		/**
		 * Set the icon to display in the 'home' section of the action bar.
		 * @param handle Drawable to show as the ActionBar icon.
		 */
		void setIcon(const MAHandle handle);

		/**
		 * Enable or disable the "home" button in the corner of the action bar.
		 * This defaults to true for packages targeting < API 14. For packages targeting
		 * API 14 or greater, the application should call this method to enable interaction
		 * with the home/up affordance.
		 * If you're using the ActionBar icon to navigate to the home activity, beware that
		 * beginning with Android API level 14, you must explicitly enable the icon as an action
		 * item by calling enableHomeButton(true).
		 * (in previous versions, the ActionBar icon was enabled as an action item by default).
		 * @param enableHomeButton True to enable the home button, False to disable the home button.
		 */
		void enableHomeButton(bool enableHomeButton);

		/**
		 * Enable or disable the icon for up navigation (which displays the "up" indicator next to the
		 * ActionBar icon.
		 * @param enableHomeAsUp True to show the user that selecting home will return one level up,
		 * rather than to the top level of the app.
		 */
		void enableHomeButtonAsUp(bool enableHomeAsUp);

		/**
		 * Set the ActionBar's background. This will be used for the primary action bar.
		 * @param image The drawable to use for the ActionBar's background.
		 */
		void setBackgroundImage(const MAHandle image);

	protected:
        /**
         * Constructor is protected since this is a singleton.
         * (subclasses can still create instances).
         */
        ActionBar();
	private:
		/**
		 * The single instance of this class.
		 */
		static ActionBar* sInstance;
	};
}

#endif /* ACTIONBAR_H_ */
