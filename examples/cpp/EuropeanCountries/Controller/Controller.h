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
