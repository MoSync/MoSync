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
 * @file ICountryDatabase.h
 * @author Bogdan Iusco
 *
 * @brief Interface to database manager used to read data from JSON files.
 */

#ifndef EC_I_COUNTRY_DATABASE_H_
#define EC_I_COUNTRY_DATABASE_H_

namespace EuropeanCountries
{
	// Forward declaration.
	class Country;

	/**
	 * @brief Interface to database manager used to read data from JSON files.
	 */
	class ICountryDatabase
	{
	public:
		/**
		 * Get the number of countries stored in database.
		 * @return Number of countries.
		 */
		virtual int countCountries() = 0;

		/**
		 * Get a country for a given id.
		 * @param id Given id.
		 * @return Country with given id if param id is valid, otherwise NULL.
		 * The ownership of the result is NOT passed to the caller.
		 */
		virtual Country* getCountryByID(const int id) = 0;

		/**
		 * Get a country for a given index.
		 * @param index Given index. Must be >=0 and < countCountries().
		 * @return Country at given index if param index is valid, otherwise NULL.
		 * The ownership of the result is NOT passed to the caller.
		 */
		virtual Country* getCountryByIndex(const int index) = 0;

	}; // end of ICountryDatabase

} // end of EuropeanCountries

#endif /* EC_I_COUNTRY_DATABASE_H_ */
