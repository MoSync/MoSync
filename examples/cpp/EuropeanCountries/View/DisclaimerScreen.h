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
 * @file DisclaimerScreen.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show the disclaimer text.
 */

#ifndef EC_COUNTRIES_DISCLAIMER_SCREEN_H_
#define EC_COUNTRIES_DISCLAIMER_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/Screen.h>

// Forward declaration for NativeUI namespace classes.
namespace NativeUI
{
	class ImageButton;
	class Label;
	class HorizontalLayout;
	class RelativeLayout;
	class VerticalLayout;
}

namespace EuropeanCountries
{

	// Forward declaration for EuropeanCountries namespace classes.
	class DisclaimerScreenObserver;

	/**
	 * @brief Screen used to show the application disclaimer.
	 */
	class DisclaimerScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param observer Observer for this screen.
		 */
		DisclaimerScreen(DisclaimerScreenObserver& observer);

		/**
		 * Destructor.
		 */
		virtual ~DisclaimerScreen();

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * @param button The button object that generated the event.
		 */
		virtual void buttonClicked(NativeUI::Widget* button);

	private:
		/**
		 * Create screen UI.
		 */
		void createUI();

		/**
		 * Create and add an Image widget to the screen's main layout.
		 * It will take the screen size.
		 */
		void createImageWidget();

		/**
		 * Create and add an HorizontalLayout to the screen's main layout.
		 * The layout will contain an back button.
		 */
		void createTitleBar();

		/**
		 * Create and add label to screen's main layout that will contain the
		 * disclaimer text.
		 */
		void createDisclaimerLabel();

	private:
		/**
		 * Observer for this screen.
		 * Will be notified when back button is pressed.
		 */
		DisclaimerScreenObserver& mObserver;

		/**
		 * Screen's main layout.
		 */
		NativeUI::RelativeLayout* mMainLayout;

		/**
		 * Show the back button.
		 */
		NativeUI::HorizontalLayout* mTitleBarLayout;

		/**
		 * Used to navigate back to the previousScreen;
		 */
		NativeUI::ImageButton* mBackButton;

		/**
		 * Show screen title.
		 */
		NativeUI::Label* mTitleLabel;

		/**
		 * Title bar height in pixels.
		 */
		int mTitleBarHeight;

	}; // end of DisclaimerScreen

} // end of EuropeanCountries

#endif /* EC_COUNTRIES_DISCLAIMER_SCREEN_H_ */
