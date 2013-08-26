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
 * @file Controller.cpp
 * @author Bogdan Iusco
 *
 * @brief Application controller.
 */

#include <NativeUI/Screen.h>
#include <MAUtil/Moblet.h>
#include <conprint.h>

#include "Controller.h"
#include "../Model/Country.h"
#include "../Model/DatabaseManager.h"
#include "../View/CountriesListScreen.h"
#include "../View/CountryInfoScreen.h"
#include "../View/DisclaimerScreen.h"
#include "../View/LoadingScreen.h"
#include "../View/ViewUtils.h"

namespace EuropeanCountries
{
	/**
	 * Constructor.
	 */
	Controller::Controller():
		mDatabaseManager(NULL),
		mCountriesListScreen(NULL),
		mCountryInfoScreen(NULL),
		mDisclaimerScreen(NULL),
		mCurrentlyShownScreen(NULL)
	{
		initScreenSizeConstants();
		initPlatformType();

		LoadingScreen* loadingScreen = NULL;
		if (isWindowsPhone())
		{
			// Show LoadingScreen.
			loadingScreen = new LoadingScreen();
			loadingScreen->show();
		}

		// Create and load the database.
		mDatabaseManager = new DatabaseManager();
		mDatabaseManager->readDataFromFiles();

		// Create screens.
		mCountriesListScreen = new CountriesListScreen(*mDatabaseManager, *this);
		mCountryInfoScreen = new CountryInfoScreen(*this);
		mDisclaimerScreen = new DisclaimerScreen(*this);

		// Show the country list screen.
		this->showScreen(*mCountriesListScreen);

		// Delete the loading screen.
		delete loadingScreen;
	}

	/**
	 * Destructor.
	 */
	Controller::~Controller()
	{
		delete mDatabaseManager;
		delete mCountriesListScreen;
		delete mCountryInfoScreen;
		delete mDisclaimerScreen;
	}

	/**
	 * Handle the back button action.
	 * Show the previous screen, or exit the app if there's no
	 * other screen to show.
	 * Platform:  Android and WP7.
	 */
	void Controller::backButtonPressed()
	{
		if (mCurrentlyShownScreen == mCountryInfoScreen)
		{
			// The current screen is country info screen, so we go back to
			// the countries list screen.
			this->showScreen(*mCountriesListScreen);
		}
		else if (mCurrentlyShownScreen == mDisclaimerScreen)
		{
			// The current screen is disclaimer screen, so we go back to
			// the country info screen.
			this->showScreen(*mCountryInfoScreen);
		}
		else
		{
			// Exit the app.
			MAUtil::Moblet::close();
		}
	}

	/**
	 * Show country info.
	 * Called after the user selects an country from the list.
	 * From CountriesListScreenObserver.
	 * @param countryID Country's id to show.
	 */
	void Controller::showCountryInfoScreen(const int countryID)
	{
		// Check if the countrID param is valid.
		Country* country = mDatabaseManager->getCountryByID(countryID);
		if (!country)
		{
			return;
		}

		// Load data.
		mCountryInfoScreen->setDisplayedCountry(*country);

		// Show the screen.
		this->showScreen(*mCountryInfoScreen);
	}

	/**
	 * Show countries list screen.
	 * Called when user taps the back button.
	 * From CountryInfoScreenObserver.
	 */
	void Controller::showCountriesListScreen()
	{
		this->showScreen(*mCountriesListScreen);
	}

	/**
	 * Show disclaimer screen.
	 * Called when user taps the info button.
	 * From CountryInfoScreenObserver.
	 */
	void Controller::showDisclaimerScreen()
	{
		this->showScreen(*mDisclaimerScreen);
	}

	/**
	 * Show country info screen.
	 * Called when user taps the back button.
	 * From DisclaimerScreenObserver.
	 */
	void Controller::showCountryInfoScreen()
	{
		this->showScreen(*mCountryInfoScreen);
	}

	/**
	 * Show a given NativeUI Screen.
	 * @param screen Screen to show.
	 */
	void Controller::showScreen(NativeUI::Screen& screen)
	{
		mCurrentlyShownScreen = &screen;
		mCurrentlyShownScreen->show();
	}

} // end of EuropeanCountries
