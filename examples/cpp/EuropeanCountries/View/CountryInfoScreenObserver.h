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
