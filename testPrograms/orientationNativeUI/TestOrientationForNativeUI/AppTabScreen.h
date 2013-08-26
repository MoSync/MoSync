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
 * @file AppTabScreen.h
 * @author Bogdan Iusco
 *
 * @brief Application's tab screen.
 */

#ifndef ORIENTATION_APP_TAB_SCREEN_H_
#define ORIENTATION_APP_TAB_SCREEN_H_

#include <NativeUI/TabScreen.h>
#include <NativeUI/TabScreenListener.h>
#include <NativeUI/ScreenListener.h>

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
		public TabScreenListener,
		public ScreenListener
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
		 * Called after the screen orientation has changed.
		 * Available only on iOS and Windows Phone 7.1 platforms.
		 */
		virtual void orientationDidChange();

		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 * @param screenOrientation The new screen orientation.
		 */
		virtual void orientationChanged(Screen* screen, int screenOrientation);
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
