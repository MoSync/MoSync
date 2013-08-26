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
