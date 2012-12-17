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
 * @file DatabaseManager.cpp
 * @author Bogdan Iusco
 *
 * @brief Database manager used to read data from JSON files.
 */

#define CHECKSUM_BUF_SIZE 128

#define BUNDLE_CHECKSUM "ECProjectChecksum"
#define COUNTRY_TABLE_FILE_NAME "CountryTable"

#include <conprint.h>
#include <mastdlib.h>
#include <yajl/YAJLDom.h>
#include <Wormhole/FileUtil.h>

#include "Country.h"
#include "DatabaseManager.h"
#include "ModelUtils.h"

namespace EuropeanCountries
{
	/**
	 * Constructor.
	 */
	DatabaseManager::DatabaseManager():
		mFileUtil(NULL),
		mCountryFileNames(NULL)
	{
		mFileUtil = new Wormhole::FileUtil();
	}

	/**
	 * Destructor.
	 */
	DatabaseManager::~DatabaseManager()
	{
		mCountryFileNames.clear();
		mCountriesMap.clear();
		delete mFileUtil;
	}

	/**
	 * Extract files from LocalFiles.bin(if needed) and read country data.
	 * This operation is synchronous and it takes time.
	 */
	void DatabaseManager::readDataFromFiles()
	{
		this->extractLocalFiles();
		this->readCountryTableFile();
		this->readAllCountryFiles();
	}

	/**
	 * Get the number of countries stored in database.
	 * @return Number of countries.
	 */
	int DatabaseManager::countCountries()
	{
		return mCountriesArray.size();
	}

	/**
	 * Get a country for a given id.
	 * @param id Given id.
	 * @return Country with given id if param id is valid, otherwise NULL.
	 * The ownership of the result is NOT passed to the caller.
	 */
	Country* DatabaseManager::getCountryByID(const int id)
	{
		Country* country = NULL;
		country = mCountriesMap[id];
		return country;
	}

	/**
	 * Get a country for a given index.
	 * @param index Given index. Must be >=0 and < countCountries().
	 * @return Country at given index if param index is valid, otherwise NULL.
	 * The ownership of the result is NOT passed to the caller.
	 */
	Country* DatabaseManager::getCountryByIndex(const int index)
	{
		Country* country = NULL;
		if (index >= 0 && index < mCountriesArray.size())
		{
			country = mCountriesArray[index];
		}
		return country;
	}

	/**
	 * Reads the CountryTable file.
	 * Data will be written into mCountryFileNames.
	 */
	void DatabaseManager::readCountryTableFile()
	{
		// Reset array.
		mCountryFileNames.clear();

		// Open CountryTable file.
		MAUtil::String filePath = mFileUtil->getLocalPath() + COUNTRY_TABLE_FILE_NAME;
		MAUtil::String fileContent;
		if (!mFileUtil->readTextFromFile(filePath, fileContent))
		{
			printf("Cannot read text from CountryTable");
			return;
		}

		//Read file content.
		MAUtil::YAJLDom::Value* root = MAUtil::YAJLDom::parse(
			(const unsigned char*)fileContent.c_str(), fileContent.size());
		MAUtil::YAJLDom::Value* countries = root->getValueForKey(sCountriesKey);
		MAUtil::YAJLDom::ArrayValue* countriesArray = (MAUtil::YAJLDom::ArrayValue*) countries;
		MAUtil::Vector<MAUtil::YAJLDom::Value*> allCountries = countriesArray->getValues();

		// Get all country files that we should read next.
		for (int index = 0; index < allCountries.size(); index++)
		{
			MAUtil::YAJLDom::Value* countryValue = allCountries[index];
			MAUtil::String countryFileName = countryValue->toString();
			mCountryFileNames.add(countryFileName);
		}
		delete root;
	}

	/**
	 * Read all country files specified in the CountryTable file.
	 * Data will be written into mCountries.
	 */
	void DatabaseManager::readAllCountryFiles()
	{
		for (int index = 0; index < mCountryFileNames.size(); index++)
		{
			MAUtil::String countryFileName = mCountryFileNames[index];
			this->readCountryFile(countryFileName);
		}
	}

	/**
	 * Read a given country file.
	 * Data will be added into mCountries.
	 * @param countryFileName File name to read.
	 */
	void DatabaseManager::readCountryFile(MAUtil::String& countryFileName)
	{
		// Open and read file content.
		MAUtil::String filePath = mFileUtil->getLocalPath() + countryFileName;
		MAUtil::String fileContent;
		if (!mFileUtil->readTextFromFile(filePath, fileContent))
		{
			printf("Cannot read text from %s", filePath.c_str());
			return;
		}

		// Extract JSON values.
		MAUtil::YAJLDom::Value* root = MAUtil::YAJLDom::parse(
			(const unsigned char*)fileContent.c_str(), fileContent.size());
		MAUtil::String name = root->getValueForKey(sCountryNameKey)->toString();
		int flagID = root->getValueForKey(sCountryFlagIDKey)->toInt();
		MAUtil::String population = root->getValueForKey(sCountryPopulationKey)->toString();
		MAUtil::String area = root->getValueForKey(sCountryAreaKey)->toString();
		MAUtil::String languages = root->getValueForKey(sCountryLanguagesKey)->toString();
		MAUtil::String government = root->getValueForKey(sCountryGovernmentKey)->toString();
		MAUtil::String capital = root->getValueForKey(sCountryCapitalKey)->toString();

		// Create and fill Country object with read data.
		Country* country = new Country();
		country->setName(name);
		country->setFlagID(flagID);
		country->setPopulation(population);
		country->setArea(area);
		country->setLanguages(languages);
		country->setGovernment(government);
		country->setCapital(capital);

		// Add object to map and array.
		mCountriesMap.insert(country->getID(), country);
		mCountriesArray.add(country);

		// Delete JSON root object.
		delete root;
	}

	/**
	 * Extract LocalFile.bin to the local file system.
	 * Files will be extracted only if the checksum does not match.
	 */
	void DatabaseManager::extractLocalFiles()
	{
		if (this->checksumHasChanged())
		{
			// Extract bundled files to the local file system.
			mFileUtil->extractLocalFiles();

			// Write the current checksum.
			this->writeChecksum();
		}
	}

	/**
	 * Check if the checksum has changed.
	 * @return true if the checksum has changed (or if the old
	 * value did not exist, such as on first time load).
	 */
	bool DatabaseManager::checksumHasChanged()
	{
		// Assume checksum has changed (or does not exist).
		bool hasChanged = true;

		// Checksum file path.
		MAUtil::String filePath = mFileUtil->getLocalPath();
		filePath += BUNDLE_CHECKSUM;

		// Read checksum of the file system bundle.
		int checksum = mFileUtil->getFileSystemChecksum(1);

		// Read checksum from file and compare.
		MAUtil::String data;
		if (mFileUtil->readTextFromFile(filePath, data))
		{
			// Read from file succeeded. Compare values.
			int existingChecksum = (int)strtol(data.c_str(), NULL, 10);
			hasChanged = checksum != existingChecksum;
		}

		return hasChanged;
	}

	/**
	 * Write the current checksum to file.
	 */
	void DatabaseManager::writeChecksum()
	{
		// Get checksum of the file system bundle.
		int checksum = mFileUtil->getFileSystemChecksum(1);

		// Checksum file path.
		MAUtil::String filePath = mFileUtil->getLocalPath();
		filePath += BUNDLE_CHECKSUM;

		// Save checksum value.
		if (checksum != 0)
		{
			char checksumBuf[CHECKSUM_BUF_SIZE];
			sprintf(checksumBuf, "%d", checksum);
			mFileUtil->writeTextToFile(filePath, checksumBuf);
		}
	}

} // end of EuropeanCountries
