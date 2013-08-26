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
