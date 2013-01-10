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
