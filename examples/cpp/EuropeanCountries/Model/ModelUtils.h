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
 * @file ModelUtils.h
 * @author Bogdan Iusco
 *
 * @brief Constants for model objects.
 */

#ifndef EC_MODEL_UTILS_H_
#define EC_MODEL_UTILS_H_

namespace EuropeanCountries
{

	/**
	 * JSON key for CountryTable file.
	 */
	extern const char* sCountriesKey;

	/**
	 * JSON keys for country files.
	 */
	extern const char* sCountryNameKey;
	extern const char* sCountryFlagIDKey;
	extern const char* sCountryPopulationKey;
	extern const char* sCountryAreaKey;
	extern const char* sCountryLanguagesKey;
	extern const char* sCountryGovernmentKey;
	extern const char* sCountryCapitalKey;

} // end of EuropeanCountries

#endif /* EC_MODEL_UTILS_H_ */
