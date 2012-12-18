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
 * @file CountriesListScreenObserver.h
 * @author Bogdan Iusco
 *
 * @brief Observer for CountriesListScreen.
 */

#ifndef EC_COUNTRIES_LIST_SCREEN_OBSERVER_H_
#define EC_COUNTRIES_LIST_SCREEN_OBSERVER_H_

namespace EuropeanCountries
{

	/**
	 * @brief Observer for CountriesListScreen.
	 */
	class CountriesListScreenObserver
	{
	public:
		/**
		 * Show country info.
		 * Called after the user selects an country from the list.
		 * @param countryID Country's id to show.
		 */
		virtual void showCountryInfoScreen(const int countryID) = 0;
	};

} // end of EuropeanCountries

#endif /* EC_COUNTRIES_LIST_SCREEN_OBSERVER_H_ */
