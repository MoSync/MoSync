/*
Copyright (C) 2011 MoSync AB

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
 * @file Functors.cpp
 * @author Gabriela Rata
 */

#include <algorithm>	//for std::accumulate
#include <numeric>		//for std::plus, std::minus, std::multiplies
#include <functional> 	//for std::divides
#include <vector>
#include "../TestSTL.h"
#include "../LOG.h"

/**
* For what is a functor see AboutFunctors.cpp
*
* STL provides definitions for several functors that are very useful in combination
* with the STL algorithms.
*
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
*
* Binary function means a function that takes with two arguments.
* Binary predicate means a function taking two arguments and returning a bool (see AboutPredicates.cpp).
*
* STL functors are defined in the <numeric> header.
*/

namespace{

void test_arithmetic_functors()
{
	//arithmetic operations:

	/**
	 *  std::plus functor defines it's function call operator as a binary function that computes the sum
	 *  of it's arguments and returns the result.
	 *  std::plus(a, b) <=> a + b
	 */
	int a = 5;
	int b = 10;
	std::plus<int> plus_fun;
	int result = plus_fun(a, b);			//5+10
	TESTIFY_ASSERT(15 == result);

	int array[] = { 2, 3, 20 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	/**
	 *  Using std::plus functor with the STL algorithm accumulate:
	 *  std::accumulate calls the functor's operator() for every element in the range.
	 *  The third argument (11) is the initial value of the result
	 *  It std::accumulate performs:
	 *  result = 11 + plus<int>(result, *array++), until it reaches
	 *  the the end of the range
	 */
	result = std::accumulate(array, array + arraySize, 11,  std::plus<int>());	//11 + (2 + 3 + 20)
	TESTIFY_ASSERT((25 + 11) == result);

	/**
	 *  std::minus functor defines it's operator() as a binary function that computes the difference
	 *  of it's arguments and returns the result.
	 *  std::minus(a, b) <=> a-b
	 */
	a = 25;
	b = 5;
	std::minus<int> minus_fun;
	result = minus_fun(a, b);
	TESTIFY_ASSERT(20 == result);		//25-5

	/**
	 *  Using std::minus functor with the STL algorithm accumulate:
	 *  std::accumulate calls the functor's operator() for every element in the range.
	 *  The third argument (26) is the initial value of the result
	 *  It performs:
	 *  result = 26 - minus(result, *array++), until it reaches
	 *  the the end of the range
	 */
	result = std::accumulate(array, array + arraySize, 26,  std::minus<int>()); //26 - 2 - 3 - 20
	TESTIFY_ASSERT(1 == result);

	/**
	 *  std::multiplies functor defines it's operator as a binary function that computes the product
	 *  of it's arguments and returns the result.
	 *  std::multiplies(a, b) <=> a*b
	 */
	a = 2;
	b = 10;
	std::multiplies<int> product_fun;
	result = product_fun(a, b);
	TESTIFY_ASSERT(20 == result);		//2*10

	/**
	 *  Using std::multiplies functor with the STL algorithm accumulate:
	 *  std::accumulate calls the functor's operator() for every element in the range.
	 *  The third argument (1) is the initial value of the result
	 *  It performs:
	 *  result = 1 * multiplies(result, *array++), until it reaches
	 *  the the end of the range
	 */
	result = std::accumulate(array, array + arraySize, 1,  std::multiplies<int>()); //1*2*3*20 = 120
	TESTIFY_ASSERT(120 == result);

	a = 10;
	b = 2;
	std::divides<int> div_fun;
	result = div_fun(a, b);
	TESTIFY_ASSERT(5 == result); // 10/2=5
	/**
	 *  Using std::divides functor with the STL algorithm accumulate:
	 *  std::accumulate calls the functor's operator() for every element in the range.
	 *  The third argument (240) is the initial value of the result
	 */
	float res = std::accumulate(array, array + arraySize, 240,
			std::divides<int>()); // array[] = {2, 3, 20}; => 240/2=120; 120/3=40; 40/20 = 2;
	TESTIFY_ASSERT(2 == res);

	a = 10;
	b = 2;
	std::modulus<int> mod_fun;
	result = mod_fun(a, b);
	TESTIFY_ASSERT(0 == result);	//10%2 = 0

	a = 22;
	std::negate<int> negateFun;
	result = negateFun(a);
	TESTIFY_ASSERT(-22 == result);
}

void test_comparison_functors()
{
	int a = 10;
	int b = 10;

	int array1[] = { 10, 20, 0 };
	int arraySize1 = sizeof(array1)/sizeof(array1[0]);

	int array2[] = { 10, 20, -1 };
	int arraySize2 = sizeof(array2)/sizeof(array2[0]);

	/**
	 *  std::equal_to
	 *  std::equal_to(a, b) <=> a == b
	 */
	TESTIFY_ASSERT( std::equal_to<int>()(a,b) == true );
	/**
	 * Using std::equal_to functor with the STL algorithm std::mismatch.
	 * std::mismatch compares the two ranges with the help of std::equal.
	 * The function returns a pair of iterators to the first element
	 * in each range for which std::equal returns false.
	 */
	std::pair<int*, int*> result = std::mismatch(array1, array1 + arraySize1,
			array2, std::equal_to<int>());

	TESTIFY_ASSERT(*result.first == 0);
	TESTIFY_ASSERT(*result.second == -1);

	/**
	 *  std::not_equal_to
	 *  std::not_equal_to(a, b) <=> a != b
	 */
	a = 3;
	b = 6;
	TESTIFY_ASSERT( std::not_equal_to<int>()(a,b) == true);
	/**
	 * Using std::not_equal_to functor with the STL algorithm std::mismatch.
	 * std::mismatch compares the two ranges with the help of std::not_equal_to.
	 * The function returns a pair of iterators to the first element
	 * in each range for which std::not_equal_to returns false.
	 */
	result = std::mismatch(array1, array1 + arraySize1,
			array2, std::not_equal_to<int>());

	TESTIFY_ASSERT(*result.first == 10);
	TESTIFY_ASSERT(*result.second == 10);

	/**
	 *  std::greater
	 *  std::greater(a, b) <=> a > b
	 */
	a = 33;
	b = 2;
	TESTIFY_ASSERT( std::greater<int>()(a,b) == true);
	/**
	 * Using std::greater functor with the STL algorithm std::mismatch.
	 * std::mismatch compares the two ranges with the help of std::greater.
	 * The function returns a pair of iterators to the first element
	 * in each range for which std::greater returns false.
	 */
	int g1[] = { 100, 200, 5 };
	int g2[] = { 11, 22, 500 };
	int g1Size = sizeof(g1)/sizeof(g1[0]);
	result = std::mismatch(g1, g1 + g1Size,
			g2, std::greater<int>());

	TESTIFY_ASSERT(*result.first == 5);
	TESTIFY_ASSERT(*result.second == 500);

	/**
	 *  std::less
	 *  std::less(a, b) <=> a < b
	 */
	a = 2;
	b = 99;
	TESTIFY_ASSERT( std::less<int>()(a,b) == true);
	/**
	 * Using std::less functor with the STL algorithm std::mismatch.
	 * std::mismatch compares the two ranges with the help of std::less.
	 * The function returns a pair of iterators to the first element
	 * in each range for which std::less returns false.
	 */
	int arr1[] = { 1, 2, 4, 120, 1001};
	int arr2[] = { 10, 20, 40 , 80, 100 };
	result = std::mismatch(arr1, arr1 + arraySize1,
			arr2, std::less<int>());
	TESTIFY_ASSERT(*result.first == 120 );
	TESTIFY_ASSERT(*result.second == 80);

	/**
	 *  std::greater_equal
	 *  std::greater(a, b) <=> a >= b
	 */
	a = 66;
	b = 10;
	TESTIFY_ASSERT( std::greater_equal<int>()(a,b) == true);

	/**
	 *  std::less_equal
	 *  std::greater(a, b) <=> a <= b
	 */
	a = 9;
	b = 55;
	TESTIFY_ASSERT( std::less_equal<int>()(a,b) == true);
}

void test_logical_operations()
{
	 bool array1[] = { true, false, true };
	 int array1Size = sizeof(array1)/sizeof(array1[0]);

	 bool array2[] = { true, true, false };
	 int array2Size = sizeof(array2)/sizeof(array2[0]);

	 bool result[3];

	/**
	 *  std::logical_and: defines "and" logical operation (&&)
	 *  std::logical_and(a, b) <=> a && b
	 */
	 TESTIFY_ASSERT(std::logical_and<int>()(array1[0], array2[0]) == true ); //true && true

	 /**
	 * Using std::logical_and functor with the STL algorithm std::transform.
	 */
	 std::transform(array1, array1 + array1Size, array2, result, std::logical_and<bool>());
	 //{true, false, true } && {true, true, false}
	 TESTIFY_ASSERT( result[0] == true ); //true && true
	 TESTIFY_ASSERT( result[1] == false ); //false && true
	 TESTIFY_ASSERT( result[2] == false ); //true && false

	/**
	 *  std::logical_or: defines "or" logical operation (||)
	 *  std::logical_and(a, b) <=> a || b
	 */
	 TESTIFY_ASSERT(std::logical_or<int>()(array1[0], array2[0]) == true);	//true && true

	/**
	 *  std::logical_not: defines "not" logical operation (!)
	 *  std::logical_not(a) <=> !a
	 */
	 TESTIFY_ASSERT(std::logical_not<int>()(array1[0]) == false );	//true && true

}

}//namespace


void TestSTL::test_functors()
{
	test_arithmetic_functors();
	test_comparison_functors();
	test_logical_operations();
}
