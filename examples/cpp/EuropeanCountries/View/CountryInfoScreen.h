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
 * @file CountryInfoScreen.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show country info.
 */

#ifndef EC_COUNTRY_INFO_SCREEN_H_
#define EC_COUNTRY_INFO_SCREEN_H_

#include <NativeUI/ButtonListener.h>
#include <NativeUI/Screen.h>

// Forward declaration for NativeUI namespace classes.
namespace NativeUI
{
	class Button;
	class HorizontalLayout;
	class ImageButton;
	class Label;
	class RelativeLayout;
	class VerticalLayout;
}

namespace EuropeanCountries
{

	// Forward declaration for EuropeanCountries namespace classes.
	class Country;
	class CountryInfoScreenObserver;

	/**
	 * Screen used to show information about a country.
	 */
	class CountryInfoScreen:
		public NativeUI::Screen,
		public NativeUI::ButtonListener
	{
	public:
		/**
		 * Constructor.
		 * @param observer Will be notified when user taps the back button.
		 */
		CountryInfoScreen(CountryInfoScreenObserver& observer);

		/**
		 * Destructor.
		 */
		virtual ~CountryInfoScreen();

		/**
		 * Show a screen.
		 */
		virtual void show();

		/**
		 * Set displayed country.
		 * Labels will be filled with data from a given country.
		 * @param country Country to display.
		 */
		void setDisplayedCountry(const Country& country);

		/**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * From ButtonListener.
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
		 */
		void createImageWidget();

		/**
		 * Create and add an HorizontalLayout to the screen's main layout.
		 * The layout will contain an back button.
		 */
		void createTitleBar();

		/**
		 * Create and add an VerticalLayout widget to the screen's main layout.
		 * The widget will be transparent.
		 */
		void createInfoLayout();

		/**
		 * Create and add two labels: first with "Population" text and second
		 * with country's population.
		 */
		void createPopulationLabel();

		/**
		 * Create and add two labels: first with "Area" text and second
		 * with country's area.
		 */
		void createAreaLabel();

		/**
		 * Create and add two labels: first with "Languages" text and second
		 * with country's languages.
		 */
		void createLanguagesLabel();

		/**
		 * Create and add two labels: first with "Government" text and second
		 * with country's government.
		 */
		void createGovernmentLabel();

		/**
		 * Create and add two labels: first with "Capital" text and second
		 * with country's capital.
		 */
		void createCapitalLabel();

		/**
		 * Create and add button used to show the disclaimer screen.
		 * Only for Android platform.
		 */
		void createDisclaimerButton();

	private:
		/**
		 * Will be notified when user taps the back button.
		 */
		CountryInfoScreenObserver& mObserver;

		/**
		 * Screen's main layout.
		 */
		NativeUI::RelativeLayout* mMainLayout;

		/**
		 * Will contain the title bar and info label.
		 * Covers the main layout.
		 * Only for Android platform.
		 */
		NativeUI::Widget* mDataLayout;

		/**
		 * Show the back button.
		 */
		NativeUI::HorizontalLayout* mTitleBarLayout;

		/**
		 * Used to display country's name.
		 */
		NativeUI::Label* mNameLabel;

		/**
		 * Relative layout where label containing country info are added.
		 * For iOS.
		 */
		NativeUI::RelativeLayout* mInfoLayoutRelative;

		/**
		 * Vertical layout where label containing country info are added.
		 * For wp7.
		 */
		NativeUI::VerticalLayout* mInfoLayoutVertical;

		/**
		 * Contains labels used to display info about the country.
		 */
		NativeUI::VerticalLayout* mInfoLayout;

		/**
		 * Used to navigate back to the countries list screen.
		 */
		NativeUI::ImageButton* mBackButton;

		/**
		 * Used to show the disclaimer screen.
		 * Only on iOS and WP7 platforms.
		 */
		NativeUI::ImageButton* mDisclaimerTitleBarButton;

		/**
		 * Used to show the country's population to the user.
		 */
		NativeUI::Label* mPopulationLabel;

		/**
		 * Used to show the country's area to the user.
		 */
		NativeUI::Label* mAreaLabel;

		/**
		 * Used to show the country's languages to the user.
		 */
		NativeUI::Label* mLanguagesLabel;

		/**
		 * Used to show the country's government to the user.
		 */
		NativeUI::Label* mGovernmentLabel;

		/**
		 * Used to show the country's capital to the user.
		 */
		NativeUI::Label* mCapitalLabel;

		/**
		 * Used to show the disclaimer screen.
		 * Only on Android platform.
		 */
		NativeUI::Button* mDisclaimerScrollAreaButton;

		/**
		 * Width value for info layout and its children.
		 */
		int mInfoLayoutWidth;

	}; // end of CountryInfoScreen

} // end of EuropeanCountries

#endif /* EC_COUNTRY_INFO_SCREEN_H_ */
