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
 * @file CountryInfoScreenObserver.h
 * @author Bogdan Iusco
 *
 * @brief Observer for CountryInfoScreenObserver.
 */

#ifndef EC_COUNTRY_INFO_SCREEN_OBSERVER_H_
#define EC_COUNTRY_INFO_SCREEN_OBSERVER_H_

namespace EuropeanCountries
{

	/**
	 * @brief Observer for CountryInfoScreenObserver.
	 */
	class CountryInfoScreenObserver
	{
	public:
		/**
		 * Show countries list screen.
		 * Called when user taps the back button.
		 */
		virtual void showCountriesListScreen() = 0;

		/**
		 * Show disclaimer screen.
		 * Called when user taps the info button.
		 */
		virtual void showDisclaimerScreen() = 0;
	};

} // end of EuropeanCountries

#endif /* EC_COUNTRY_INFO_SCREEN_OBSERVER_H_ */
