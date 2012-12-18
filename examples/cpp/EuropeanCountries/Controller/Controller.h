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
 * @file Controller.h
 * @author Bogdan Iusco
 *
 * @brief Application controller.
 */

#ifndef EC_CONTROLLER_H_
#define EC_CONTROLLER_H_

#include "../View/CountriesListScreenObserver.h"
#include "../View/CountryInfoScreenObserver.h"
#include "../View/DisclaimerScreenObserver.h"

// Forward declarations for NativeUI namespace classes.
namespace NativeUI
{
	class Screen;
}

namespace EuropeanCountries
{

	// Forward declarations for NativeUI EuropeanCountries classes.
	class CountriesListScreen;
	class Country;
	class CountryInfoScreen;
	class DatabaseManager;
	class DisclaimerScreen;

	/**
	 * @brief Application controller.
	 */
	class Controller:
		public CountriesListScreenObserver,
		public CountryInfoScreenObserver,
		public DisclaimerScreenObserver
	{
	public:
		/**
		 * Constructor.
		 */
		Controller();

		/**
		 * Destructor.
		 */
		virtual ~Controller();

		/**
		 * Handle the back button action.
		 * Show the previous screen, or exit the app if there's no
		 * other screen to show.
		 * Platform:  Android and WP7.
		 */
		void backButtonPressed();

		/**
		 * Show country info.
		 * Called after the user selects an country from the list.
		 * From CountriesListScreenObserver.
		 * @param countryID Country's id to show.
		 */
		virtual void showCountryInfoScreen(const int countryID);

		/**
		 * Show countries list screen.
		 * Called when user taps the back button.
		 * From CountryInfoScreenObserver.
		 */
		virtual void showCountriesListScreen();

		/**
		 * Show disclaimer screen.
		 * Called when user taps the info button.
		 * From CountryInfoScreenObserver.
		 */
		virtual void showDisclaimerScreen();

		/**
		 * Show country info screen.
		 * Called when user taps the back button.
		 * From DisclaimerScreenObserver.
		 */
		virtual void showCountryInfoScreen();

	private:
		/**
		 * Show a given NativeUI Screen.
		 * @param screen Screen to show.
		 */
		void showScreen(NativeUI::Screen& screen);

	private:
		/**
		 * Used to read data from files.
		 */
		DatabaseManager* mDatabaseManager;

		/**
		 * Screen used to display all countries in a list.
		 */
		CountriesListScreen* mCountriesListScreen;

		/**
		 * Screen used to display country info.
		 */
		CountryInfoScreen* mCountryInfoScreen;

		/**
		 * Screen used to display the disclaimer.
		 */
		DisclaimerScreen* mDisclaimerScreen;

		/**
		 * Points to the currently visible screen.
		 * Not owns the pointed object.
		 */
		NativeUI::Screen* mCurrentlyShownScreen;

	}; // end of Controller

} // end of EuropeanCountries

#endif /* EC_CONTROLLER_H_ */
