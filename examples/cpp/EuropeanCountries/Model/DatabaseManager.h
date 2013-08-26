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
 * @file DatabaseManager.h
 * @author Bogdan Iusco
 *
 * @brief Database manager used to read data from JSON files.
 */

#ifndef EC_DATABASE_MANAGER_H_
#define EC_DATABASE_MANAGER_H_

#include <MAUtil/Map.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

#include "ICountryDatabase.h"

// Forward declarations for Wormhole namespace classes.
namespace Wormhole
{
	class FileUtil;
}

namespace EuropeanCountries
{

	// Forward declarations for EuropeanCountries namespace classes.
	class Country;

	/**
	 * @brief Handles read operation from files.
	 */
	class DatabaseManager:
		public ICountryDatabase
	{
	public:
		/**
		 * Constructor.
		 */
		DatabaseManager();

		/**
		 * Destructor.
		 */
		~DatabaseManager();

		/**
		 * Extract files from LocalFiles.bin(if needed) and read country data.
		 * This operation is synchronous and it takes time.
		 */
		void readDataFromFiles();

		/**
		 * Get the number of countries stored in database.
		 * @return Number of countries.
		 */
		virtual int countCountries();

		/**
		 * Get a country for a given id.
		 * @param id Given id.
		 * @return Country with given id if param id is valid, otherwise NULL.
		 * The ownership of the result is NOT passed to the caller.
		 */
		virtual Country* getCountryByID(const int id);

		/**
		 * Get a country for a given index.
		 * @param index Given index. Must be >=0 and < countCountries().
		 * @return Country at given index if param index is valid, otherwise NULL.
		 * The ownership of the result is NOT passed to the caller.
		 */
		virtual Country* getCountryByIndex(const int index);

	private:

		/**
		 * Reads the CountryTable file.
		 * Data will be written into mCountryFileNames.
		 */
		void readCountryTableFile();

		/**
		 * Read all country files specified in the CountryTable file.
		 * Data will be written into mCountries.
		 */
		void readAllCountryFiles();

		/**
		 * Read a given country file.
		 * Data will be added into mCountries.
		 * @param countryFileName File name to read.
		 */
		void readCountryFile(MAUtil::String& countryFileName);

		/**
		 * Extract LocalFile.bin to the local file system.
		 * Files will be extracted only if the checksum does not match.
		 */
		void extractLocalFiles();

		/**
		 * Check if the checksum has changed.
		 * @return true if the checksum has changed (or if the old
		 * value did not exist, such as on first time load).
		 */
		bool checksumHasChanged();

		/**
		 * Write the current checksum to file.
		 */
		void writeChecksum();

	private:
		/**
		 * Used to extract LocalFiles.bin
		 */
		Wormhole::FileUtil* mFileUtil;

		/**
		 * Holds data from CountryTable file.
		 */
		MAUtil::Vector<MAUtil::String> mCountryFileNames;

		/**
		 * Holds data from all country files.
		 * Int value represents the key(coutry's id value).
		 */
		MAUtil::Map<int, Country*> mCountriesMap;

		/**
		 * Holds data from all country files.
		 * Contains same data as mCountriesMap, but for performance reasons data
		 * will be stored in vector too.
		 */
		MAUtil::Vector<Country*> mCountriesArray;

	}; // end of DatabaseManager

} // end of EuropeanCountries

#endif /* EC_DATABASE_MANAGER_H_ */
