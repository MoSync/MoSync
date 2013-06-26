/*
 * FunctionAdaptors.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
 */


#include <functional>
#include <algorithm>
#include <vector>
#include "../TestSTL.h"
#include "../Log.h"

/**
* Function object adaptors are used to create a function object from another function object.
* The created function object, is not the same as the original functor, but is adapted to a certain need.
* For example if we have a function object like std::less, and we want to compare all the elements
* in a range against 10, then be can use an STL adaptor (bind2nd), that bounds the second argument
* of std::less to 10. Then we can use std::less with algorithms like remove_copy_if,
* that need a unary predicate.
*
* STL provides two functor adaptors: bind1st and bind2nd.
*
* 	bind1st: constructs an unary function object from a binary function object, by binding
* 	the first argument to a fixed value.
*
* 	bind1st template function is defined like this:
*
*   binder1st<SomeFunctor> bind1st (const SomeFunctor& fun, const T& fixedValue)
*	{
*		return binder1st<SomeFunctor>(fun, x);
*	}
*
*	bind1st returns an binder1st object, which is actually a functor that forwards the function calls
*	to the "fun" argument it takes as a parameter, when constructed:
*
*	template <class Functor> class binder1st {
*
*		binder1st(const Functor &fun, Functor::first_argument_type &fixed)
*		{
*			mFun = fun;
*			mFixedValue = fixed;
*		}
*
*		Functor::result_type operator()(Functor::second_argument_type &someValue){
*			return mFun(mFixedValue, someValue);
*		}
*	};
*
*	bind2nd is implemented in a similar way, but instead of binding the first argument,
*	bind2nd it will bind the second one to a fixed value.
*
*	bind1st and bind2nd function templates are defined in the <functional> header.
*
*/

void TestSTL::test_adaptors()
{
	/**
	*  bind1st function template: constructs an unary function object from a binary function object,
	*  by binding the first parameter to a certain value.
	*/

	int array[] = { 1, -99, 2, -100 };
	int arraySize = sizeof(array)/sizeof(array[0]);

	std::vector<int> vec1(array, array + arraySize);
	std::vector<int>::iterator newEnd;
	//Calls std::less(0, element). If less(0,element) returns true => remove that element
	newEnd = std::remove_if(vec1.begin(), vec1.end(), bind1st(std::less<int>(), 0));

	for(std::vector<int>::iterator it = vec1.begin(); it != newEnd; ++it)
	{
		TESTIFY_ASSERT(*it < 0);
	}

	/**
	*  bind2nd function template: constructs an unary function object from a binary function object,
	*  by binding the second parameter to a certain value.
	*/
	std::vector<int> vec2(array, array + arraySize);

	//Calls std::greater(element, 0). If element greater(element, 0) returns true => remove that element
	newEnd = std::remove_if(vec2.begin(), vec2.end(), bind2nd(std::greater<int>(), 0));
	for(std::vector<int>::iterator it = vec2.begin(); it != newEnd; ++it)
	{
		TESTIFY_ASSERT(*it < 0);
	}
}
