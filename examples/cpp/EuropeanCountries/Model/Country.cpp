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
 * @file Country.cpp
 * @author Bogdan Iusco
 *
 * @brief Class used to store information about a country.
 */

#include "Country.h"

namespace EuropeanCountries
{

	/**
	 * Constructor.
	 * Automatically creates an id for the returned object.
	 */
	Country::Country()
	{
		mID = maCreatePlaceholder();
	}

	/**
	 * Destructor.
	 */
	Country::~Country()
	{
		maDestroyPlaceholder(mID);
	}

	/**
	 * Get country's id.
	 * The id is automatically created and it cannot be changed.
	 * @return Country's ID.
	 */
	int Country::getID() const
	{
		return mID;
	}

	/**
	 * Set country flag id.
	 * @param flagID Image handle to a resource file.
	 */
	void Country::setFlagID(const int flagID)
	{
		mFlagImage = flagID;
	}

	/**
	 * Get flag image ID.
	 * @return Image handle to a resource file.
	 */
	int Country::getFlagID() const
	{
		return mFlagImage;
	}

	/**
	 * Set country name.
	 * @param name Name to set.
	 */
	void Country::setName(const MAUtil::String& name)
	{
		mName = name;
	}

	/**
	 * Get country name.
	 * @return Country name.
	 */
	MAUtil::String Country::getName() const
	{
		return mName;
	}

	/**
	 * Set country population.
	 * @param population Text to set.
	 */
	void Country::setPopulation(const MAUtil::String& population)
	{
		mPopulation = population;
	}

	/**
	 * Get country population.
	 * @return Country name.
	 */
	MAUtil::String Country::getPopulation() const
	{
		return mPopulation;
	}

	/**
	 * Set country area.
	 * @param area Text to set.
	 */
	void Country::setArea(const MAUtil::String& area)
	{
		mArea = area;
	}

	/**
	 * Get country area.
	 * @return Country area.
	 */
	MAUtil::String Country::getArea() const
	{
		return mArea;
	}

	/**
	 * Set country languages.
	 * @param languages Text to set.
	 */
	void Country::setLanguages(const MAUtil::String& languages)
	{
		mLanguages = languages;
	}

	/**
	 * Get country languages.
	 * @return Country languages.
	 */
	MAUtil::String Country::getLanguages() const
	{
		return mLanguages;
	}

	/**
	 * Set country government.
	 * @param government Text to set.
	 */
	void Country::setGovernment(const MAUtil::String& government)
	{
		mGovernment = government;
	}

	/**
	 * Get country government.
	 * @return Country government.
	 */
	MAUtil::String Country::getGovernment() const
	{
		return mGovernment;
	}

	/**
	 * Set country capital.
	 * @param capital Text to set.
	 */
	void Country::setCapital(const MAUtil::String& capital)
	{
		mCapital = capital;
	}

	/**
	 * Get country capital.
	 * @return Country capital.
	 */
	MAUtil::String Country::getCapital() const
	{
		return mCapital;
	}

} // end of EuropeanCountries
