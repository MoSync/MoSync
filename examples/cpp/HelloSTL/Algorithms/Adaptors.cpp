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
 * @file FunctionAdaptors.cpp
 * @author Gabriela Rata
 */

#include <functional>
#include <algorithm>
#include <vector>
#include "../STLMoblet.h"
#include "../Log.h"

/**
* Example using STL adaptors
*/
void STLMoblet::STL_adaptors()
{
	LOG("\n");
	LOG("========================= STL adaptors ==========================================================================");

	LOG("/**");
	LOG("* Function object adaptors are used to create a function object from another function object."				);
	LOG("* The created function object, is not the same as the original functor, but is adapted to a certain need."	);
	LOG("* For example if we have a function object like std::less, and we want to compare all the elements"		);
	LOG("* in a range against 10, then be can use an STL adaptor (bind2nd), that bounds the second argument"		);
	LOG("* of std::less to 10. Then we can use std::less with algorithms like remove_copy_if,"						);
	LOG("* that need a unary predicate."																			);
	LOG("*"																											);
	LOG("* STL provides two functor adaptors: bind1st and bind2nd."													);
	LOG("*"																											);
	LOG("* 	bind1st: constructs an unary function object from a binary function object, by binding"					);
	LOG("* 	the first argument to a fixed value."																	);
	LOG("*"																											);
	LOG("* 	bind1st template function is defined like this:"														);
	LOG("*"																											);
	LOG("*   binder1st<SomeFunctor> bind1st (const SomeFunctor& fun, const T& fixedValue"							);
	LOG("*	{"																										);
	LOG("*		return binder1st<SomeFunctor>(fun, x);"																);
	LOG("*	}"																										);
	LOG("*"																											);
	LOG("*	bind1st returns an binder1st object, which is actually a functor that forwards the function calls"		);
	LOG("*	to the \"fun\" argument it takes as a parameter, when constructed:"										);
	LOG("*"																											);
	LOG("*	template <class Functor> class binder1st {"																);
	LOG("*"																											);
	LOG("*		binder1st(const Functor &fun, Functor::first_argument_type &fixed)"									);
	LOG("*		{"																									);
	LOG("*			mFun = fun;"																					);
	LOG("*			mFixedValue = fixed;"																			);
	LOG("*		}"																									);
	LOG("*"																											);
	LOG("*		Functor::result_type operator()(Functor::second_argument_type &someValue){"							);
	LOG("*			return mFun(mFixedValue, someValue);"															);
	LOG("*		}"																									);
	LOG("*	};"																										);
	LOG("*"																											);
	LOG("*	bind2nd is implemented in a similar way, but instead of binding the first argument,"					);
	LOG("*	bind2nd it will bind the second one to a fixed value."													);
	LOG("*"																											);
	LOG("*	bind1st and bind2nd function templates are defined in the <functional> header."							);
	LOG("*/");
	LOG("\n");

	LOG("				Example using adaptors 							  ");


	LOG("\n																				   ");
	LOG("/**"																				);
	LOG("*  bind1st function template: constructs an unary function object from a"			);
	LOG("*  binary function object, by binding the first parameter to a certain value."		);
	LOG("\n    */"																			);

	log_to_console("\n     Example using std::bind1st:\n");

	int array[] = { 1, -99, 2, -100 };
	int arraySize = sizeof(array)/sizeof(array[0]);

	std::vector<int> vec1(array, array + arraySize);
	log_to_console(vec1, "vec1 contains: ");

	LOG("\n"																		);
	LOG("/**std::remove_if calls std::less(0, element). If less(0,element) returns"	);
	LOG("*  true => removes that element."											);
	LOG("*  less(0,element) is equivalent to 0<element."							);
	LOG("*/"																		);
	LOG("\n"																		);

	TRACE(std::vector<int>::iterator newEnd = std::remove_if(vec1.begin(),
			vec1.end(), bind1st(std::less<int>(), 1)));

	log_to_console("vec1 after calling std::remove_if(vec1.begin(), vec1.end(), "
			"bind1st(std::less<int>(), 0)): ");
	for(std::vector<int>::iterator it = vec1.begin(); it != newEnd; ++it)
	{
		log_to_console(*it);
	}


	LOG("\n"															      	  );
	LOG("/**"																  	  );
	LOG("*  bind2nd function template: constructs an unary function object from a");
	LOG("*  binary function object, by binding the second parameter to a certain" );
	LOG("*  value."															 	  );
	LOG("*/"																	  );

	log_to_console("\n     Example using std::bind2nd:\n");

	std::vector<int> vec2(array, array + arraySize);
	log_to_console(vec2, "vec2 contains: ");

	LOG("\n"																	  );
	LOG("/**  std::remove_if calls std::greater(element, 0)."					  );
	LOG("*  If std::greater(element, 0) returns true => removes that element."	  );
	LOG("*  std::greater(element, 0) is equivalent to element > 0"				  );
	LOG("*/"																	  );
	LOG("\n");

	TRACE(newEnd = std::remove_if(vec2.begin(), vec2.end(),
			bind2nd(std::greater<int>(), 0)));

	log_to_console("vec2 after calling: std::remove_if(vec2.begin(), vec2.end(), "
			"bind2nd(std::greater<int>(), 0));");

	for(std::vector<int>::iterator it = vec2.begin(); it != newEnd; ++it)
	{
		log_to_console(*it);
	}
	LOG("\n");
}
