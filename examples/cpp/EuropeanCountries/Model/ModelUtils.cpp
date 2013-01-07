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
 * @file ModelUtils.cpp
 * @author Bogdan Iusco
 *
 * @brief Constants for model objects.
 */

#include "ModelUtils.h"

namespace EuropeanCountries
{

	/**
	 * JSON key for CountryTable file.
	 */
	const char* sCountriesKey = "countries";

	/**
	 * JSON keys for country files.
	 */
	const char* sCountryNameKey = "Name";
	const char* sCountryFlagIDKey = "FlagID";
	const char* sCountryPopulationKey = "Population";
	const char* sCountryAreaKey = "Area";
	const char* sCountryLanguagesKey = "Languages";
	const char* sCountryGovernmentKey = "Government";
	const char* sCountryCapitalKey = "Capital";

} // end of EuropeanCountries
