/*
 * Functors.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
 */

#include <algorithm>
#include <numeric>
#include "../TestSTL.h"

/**
* STL provides definitions for several functors that are very useful in combination
* with the STL algorithms.
*
* Binary function means a function that takes with two arguments.
* Binary predicate means a function taking two arguments and returning a bool (see AboutPredicates.cpp).
* STL functors define:
*
* 		- arithmetic operations:	plus 			( binary function <=> arg1 +  arg2 )
*									minus 			( binary function <=> arg1 -  arg2 )
*									multiplies		( binary function <=> arg1 *  arg2 )
*									divides			( binary function <=> arg1 /  arg2 )
*									modulus			( binary function <=> arg1 %  arg2 )
*									negate			( unary function <=> -arg )
*
* 		- comparison operations: 	equal_to		( binary predicate <=> arg1 ==  arg2 )
*									not_equal_to	( binary predicate <=> arg1 !=  arg2 )
*									greater			( binary predicate <=> arg1 >  arg2  )
*									less			( binary predicate <=> arg1 <  arg2  )
*									greater_equal	( binary predicate <=> arg1 >=  arg2 )
*									less_equal		( binary predicate <=> arg1 <=  arg2 )
*
* 		- logical operations:		logical_and		( binary predicate <=> arg1 &&  arg2 )
*									logical_or		( binary predicate <=> arg1 ||  arg2 )
*									logical_not		( unary predicate <=> !arg )
*/

void test_functors()
{
	//arithmetic operations:

	/**
	 *  std::plus functor defines it's operator as a binary function that computes the sum
	 *  of it's arguments and returns the result.
	 */
	int a = 5;
	int b = 10;
	std::plus<int> fun;
	int result = fun(a, b);
	TESTIFY_ASSERT(15 == result);

	int array[] = { 2, 3, 20 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	/**
	 *  Adds all the elements in the array
	 *  The third argument (zero) is the initial value of the resulting sum
	 *  It performs:
	 *  result = std::plus<int>(result + *array++), until it reaches
	 *  the the end of the range
	 */
	result = std::accumulate(array, array + arraySize, 0,  std::plus<int>());
	TESTIFY_ASSERT(25 == result);

}
