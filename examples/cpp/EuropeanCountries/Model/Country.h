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
 * @file Country.h
 * @author Bogdan Iusco
 *
 * @brief Class used to store information about a country.
 */

#ifndef EC_COUNTRY_H_
#define EC_COUNTRY_H_

#include <MAUtil/String.h>

namespace EuropeanCountries
{

	class Country
	{
	public:
		/**
		 * Constructor.
		 * Automatically creates an id for the returned object.
		 */
		Country();

		/**
		 * Destructor.
		 */
		~Country();

		/**
		 * Get country's id.
		 * The id is automatically created and it cannot be changed.
		 * @return Country's ID.
		 */
		int getID() const;

		/**
		 * Set country flag id.
		 * @param flagID Image handle to a resource file.
		 */
		void setFlagID(const int flagID);

		/**
		 * Get flag image ID.
		 * @return Image handle to a resource file.
		 */
		int getFlagID() const;

		/**
		 * Set country name.
		 * @param name Name to set.
		 */
		void setName(const MAUtil::String& name);

		/**
		 * Get country name.
		 * @return Country name.
		 */
		MAUtil::String getName() const;

		/**
		 * Set country population.
		 * @param population Text to set.
		 */
		void setPopulation(const MAUtil::String& population);

		/**
		 * Get country population.
		 * @return Country name.
		 */
		MAUtil::String getPopulation() const;

		/**
		 * Set country area.
		 * @param area Text to set.
		 */
		void setArea(const MAUtil::String& area);

		/**
		 * Get country area.
		 * @return Country area.
		 */
		MAUtil::String getArea() const;

		/**
		 * Set country languages.
		 * @param languages Text to set.
		 */
		void setLanguages(const MAUtil::String& languages);

		/**
		 * Get country languages.
		 * @return Country languages.
		 */
		MAUtil::String getLanguages() const;

		/**
		 * Set country government.
		 * @param government Text to set.
		 */
		void setGovernment(const MAUtil::String& government);

		/**
		 * Get country government.
		 * @return Country government.
		 */
		MAUtil::String getGovernment() const;

		/**
		 * Set country capital.
		 * @param capital Text to set.
		 */
		void setCapital(const MAUtil::String& capital);

		/**
		 * Get country capital.
		 * @return Country capital.
		 */
		MAUtil::String getCapital() const;

	private:

		/**
		 * Country's id.
		 */
		int mID;

		/**
		 * Country flag id.
		 * Represents an image handle from app resources.
		 */
		int mFlagImage;

		/**
		 * Country name.
		 */
		MAUtil::String mName;

		/**
		 * Country population.
		 */
		MAUtil::String mPopulation;

		/**
		 * Country area.
		 */
		MAUtil::String mArea;

		/**
		 * Country languages.
		 */
		MAUtil::String mLanguages;

		/**
		 * Country government.
		 */
		MAUtil::String mGovernment;

		/**
		 * Country capital.
		 */
		MAUtil::String mCapital;

	}; // end of Country

} // end of EuropeanCountries

#endif /* EC_COUNTRY_H_ */
