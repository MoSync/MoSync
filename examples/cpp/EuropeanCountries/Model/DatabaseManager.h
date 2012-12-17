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
