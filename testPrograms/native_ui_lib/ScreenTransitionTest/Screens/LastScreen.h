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
        void resetTitleWithString(const char* aString);

	private:
		/**
		 * Create screen's UI.
		 */
		void createUI();

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
