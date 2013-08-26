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
 * @file SecondScreen.h
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief Last screen.
 */

#ifndef LAST_SCREEN_H_
#define LAST_SCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ButtonListener.h>

#include "../Observers/LastScreenObserver.h"

namespace NativeUI
{
	class Button;
	class Label;
	class VerticalLayout;
}

namespace ScreenTransitionTest
{

	class LastScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param observer Observer for this screen.
		 */
		LastScreen(LastScreenObserver& observer);

		/**
		 * Destructor.
		 */
		virtual ~LastScreen();

        /**
         * This method is called if the touch-up event was inside the
         * bounds of the button.
         * Platform: iOS, Android, Windows Phone.
         * @param button The button object that generated the event.
         */
        virtual void buttonClicked(NativeUI::Widget* button);

		/**
		 * Appends text to title.
		 */
        virtual void resetTitleWithString(const char* aString);

	private:
		/**
		 * Create screen's UI.
		 */
        virtual void createUI();

        virtual void addInfoLabel();

        virtual void addTransitionInfoLabels();

        virtual void addFooter();

	private:
		/**
		 * Observer for this screen.
		 */
		LastScreenObserver& mObserver;

		/**
		 * Screen's layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;

		/**
		 * Hide this screen on click.
		 */
		NativeUI::Button* mHideScreenButton;

		/**
		 * Screen Title
		 */
		NativeUI::Label* mTransitionNameLabel;

		/**
		 * Footer layout.
		 */
		NativeUI::HorizontalLayout* mFooterLayout;
	};
} // end of ScreenTransitionTest

#endif /* LAST_SCREEN_H_ */
