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
 * @file AppTabScreen.h
 * @author Bogdan Iusco
 *
 * @brief Application's tab screen.
 */

#ifndef ORIENTATION_APP_TAB_SCREEN_H_
#define ORIENTATION_APP_TAB_SCREEN_H_

#include <NativeUI/TabScreen.h>
#include <NativeUI/TabScreenListener.h>

namespace OrientationTest
{

	typedef enum
	{
		TAB_SCREEN_ONE = 0,
		TAB_SCREEN_TWO
	} VisibleTabScreen;

	using namespace NativeUI;

	// Forward declarations.
	class FirstScreen;
	class SecondScreen;

	class AppTabScreen:
		public TabScreen,
		public TabScreenListener
	{
	public:
		/**
		 * Constructor.
		 */
		AppTabScreen();

		/**
		 * Destructor.
		 */
		virtual ~AppTabScreen();

		/**
		 * Called just before the screen begins rotating.
		 */
		virtual void orientationWillChange();

		/**
		 * Called after the screen orientation has changed.
		 * Available only on iOS and Windows Phone 7.1 platforms.
		 */
		virtual void orientationDidChange();

	private:
		/**
		 * This method is called when a tab screen has changed to a new tab.
		 * @param tabScreen The tab screen object that generated the event.
		 * @param tabScreenIndex The index of the new tab.
		 */
		virtual void tabScreenTabChanged(
			TabScreen* tabScreen,
			const int tabScreenIndex);

	private:
		/**
		 * First tab screen.
		 */
		FirstScreen* mFirstScreen;

		/**
		 * Second tab screen.
		 */
		SecondScreen* mSecondScreen;

		/**
		 * Store the visible screen index.
		 */
		VisibleTabScreen mVisibleTabScreen;
	};

} // namespace OrientationTest

#endif /* ORIENTATION_APP_TAB_SCREEN_H_ */
