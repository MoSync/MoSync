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
 * @file AuxiliaryFunctions.cpp
 * @author Gabriela Rata
 */


#include <algorithm>
#include "../STLMoblet.h"
#include "../LOG.h"
#include "../DummyClass.h"
#include "../Employee.h"


/**
 * Function for explaining auxiliary functions from the <algorithm> header: min, max, swap
 */
void STLMoblet::aux_functions()
{
	LOG("\n");
	LOG("========================= min, max and swap functions (found in <algorithm> header) =============================");
	LOG("/**");
	LOG(" * min, max template functions: If both values are equal, usually the first element is returned");
	LOG(" * 	Both functions are provided with an additional argument, that is the comparison criterion.");
	LOG(" * 	The default comparison criterion is operator<.");
	LOG(" * swap: provided to swap the values of two objects.");
	LOG(" * 	(The swap is possible only if the copy constructor and the assignment operators are defined.)");
	LOG(" */");
	LOG("\n");

	TRACE(int small = 10;);
	TRACE(int big = 20;);

	int resMin = std::min(small, big);
	log_to_console(resMin, "int resMin = std::min(small, big);	//resMin = ");

	int resMax = std::max(small, big);
	log_to_console(resMax, "int resMax = std::max(small, big);	//resMax = ");
	log_to_console("\n");

	TRACE(Employee junior(1, "Jimmy"););
	TRACE(Employee bigBoss(12, "Alex"););

	Employee smallE = std::min(junior, bigBoss, lessExperiencePredicate);

	log_to_console(smallE, "Employee smaller = std::min(junior, bigBoss, "
			"lessExperience);	//smallE = ");

	Employee bigE = std::max(junior, bigBoss, lessExperiencePredicate);
	log_to_console(bigE, "Employee big = std::max(junior, bigBoss, lessExperience);	//bigE = ");
	log_to_console("\n");

	TRACE(std::swap(smallE, bigE));
	log_to_console(smallE, "smallE after swap:  ");
	log_to_console(bigE, "bigE after swap: ");

	LOG("\n");
}
