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
#include "../STLMoblet.h"
#include "../LOG.h"



namespace{

void arithmetic_functors()
{
	LOG("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	LOG("		arithmetic functors							");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	LOG("/**");
	LOG(" *  std::plus functor defines it's function call operator as a binary function that computes the sum");
	LOG(" *  of it's arguments and returns the result.");
	LOG(" *  std::plus(a, b) <=> a + b");
	LOG(" */");
	LOG("\n");

	LOG("\n");
	TRACE(int a = 5;);
	TRACE(int b = 10;);
	TRACE(std::plus<int> plus_fun;);
	int result = plus_fun(a, b);
	log_to_console(result, "int result = plus_fun(a, b);	//result = "); 			//5+10


	LOG("/**");
	LOG(" *  Using std::plus functor with the STL algorithm accumulate:);");
	LOG(" *  std::accumulate calls the functor's operator() for every element in the range.");
	LOG(" *  The third argument (11) is the initial value of the result");
	LOG(" *  It std::accumulate performs:");
	LOG(" *  result = 11 + plus<int>(result, *array++), until it reaches");
	LOG(" *  the the end of the range");
	LOG(" */");

	int array[] = { 2, 3, 20 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	log_to_console(array, arraySize, "\n     array contains: ");

	result = std::accumulate(array, array + arraySize, 11,  std::plus<int>());	//11 + (2 + 3 + 20)
	log_to_console(result, "result = std::accumulate(array, "
			"array + arraySize, 11,  std::plus<int>());	//result = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::minus functor defines it's operator() as a binary function that computes the difference");
	LOG(" *  of it's arguments and returns the result.");
	LOG(" *  std::minus(a, b) <=> a-b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 25);
	TRACE(b = 5);
	TRACE(std::minus<int> minus_fun);
	result = minus_fun(a, b);
	log_to_console(result, "result = minus_fun(a, b);	//result = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  Using std::minus functor with the STL algorithm accumulate:");
	LOG(" *  std::accumulate calls the functor's operator() for every element in the range.");
	LOG(" *  The third argument (26) is the initial value of the result");
	LOG(" *  It performs:");
	LOG(" *  result = 26 - minus(result, *array++), until it reaches");
	LOG(" *  the the end of the range");
	LOG(" */");
	LOG("\n");

	result = std::accumulate(array, array + arraySize, 26,  std::minus<int>()); //26 - 2 - 3 - 20
	log_to_console(result, "result = std::accumulate(array, array + arraySize,"
			" 26,  std::minus<int>());	//performs: 26 - 2 - 3 - 20; 	result = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::multiplies functor defines it's operator as a binary function that computes the product");
	LOG(" *  of it's arguments and returns the result.");
	LOG(" *  std::multiplies(a, b) <=> a*b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 2;);
	TRACE(b = 10;);
	TRACE(std::multiplies<int> product_fun;);
	result = product_fun(a, b);
	log_to_console(result, "result = product_fun(a, b);	//result = ");


	LOG("\n");
	LOG("/**");
	LOG(" *  Using std::multiplies functor with the STL algorithm accumulate:");
	LOG(" *  std::accumulate calls the functor's operator() for every element in the range.");
	LOG(" *  The third argument (1) is the initial value of the result");
	LOG(" *  It performs:");
	LOG(" *  result = 1 * multiplies(result, *array++), until it reaches");
	LOG(" *  the the end of the range");
	LOG(" */");
	LOG("\n");

	result = std::accumulate(array, array + arraySize, 1,  std::multiplies<int>()); //1*2*3*20 = 120
	log_to_console(result, "result = std::accumulate(array, array + arraySize,"
			" 1, std::multiplies<int>());	//performs: 1*2*3*20 = 120; result = ");


	TRACE(a = 10;);
	TRACE(b = 2;);
	TRACE(std::divides<int> div_fun;);
	result = div_fun(a, b);
	log_to_console(result, "result = div_fun(a, b);	//result = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  Using std::divides functor with the STL algorithm accumulate:");
	LOG(" *  std::accumulate calls the functor's operator() for every element in the range.");
	LOG(" *  The third argument (240) is the initial value of the result");
	LOG(" */");
	LOG("\n");

	float res = std::accumulate(array, array + arraySize, 240,
			std::divides<int>()); // array[] = {2, 3, 20}; => 240/2=120; 120/3=40; 40/20 = 2;

	log_to_console(array, arraySize, "array contains: ");
	log_to_console(res, "float res = std::accumulate(array, array + arraySize, "
			"240, std::divides<int>());	//performs: 240/2=120; 120/3=40; 40/20 = 2; "
			" result = ");
	log_to_console("\n");

	TRACE(a = 10;);
	TRACE(b = 2;);
	TRACE(std::modulus<int> mod_fun;);
	result = mod_fun(a, b);
	log_to_console(result, "result = mod_fun(a, b);	//performs: 10%2; 	result = ");
	log_to_console("\n");

	TRACE(a = 22;);
	TRACE(std::negate<int> negateFun;);
	result = negateFun(a);
	log_to_console(result, "result = negateFun(a);	//result = ");
}

void comparison_functors()
{
	LOG("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	LOG("			comparison functors									  ");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");


//	int a = 10;
//	int b = 10;

	LOG("/**");
	LOG(" *  std::equal_to");
	LOG(" *  std::equal_to(a, b) <=> a == b");
	LOG(" */");
	LOG("\n");

	TRACE(int a = 10;);
	TRACE(int b = 10;);
	bool res = std::equal_to<int>()(a,b);
	log_to_console((bool)res, "std::equal_to<int>()(a,b); returns: ");
	log_to_console("\n");

	int array1[] = { 10, 20, 0 };
	int arraySize1 = sizeof(array1)/sizeof(array1[0]);
	log_to_console(array1, arraySize1, "array1 contains: ");

	int array2[] = { 10, 20, -1 };
	int arraySize2 = sizeof(array2)/sizeof(array2[0]);
	log_to_console(array2, arraySize1, "array2 contains: ");
	log_to_console("\n");

	LOG("/**");
	LOG(" * Using std::equal_to functor with the STL algorithm std::mismatch.");
	LOG(" * std::mismatch compares the two ranges with the help of std::equal.");
	LOG(" * The function returns a pair of iterators to the first element");
	LOG(" * in each range for which std::equal returns false.");
	LOG(" */");
	LOG("\n");

	std::pair<int*, int*> result = std::mismatch(array1, array1 + arraySize1,
			array2, std::equal_to<int>());

	log_to_console("std::pair<int*, int*> result = std::mismatch(array1, "
			"array1 + arraySize1, array2, std::equal_to<int>());");
	log_to_console(*result.first, "*result.first = ");
	log_to_console(*result.second, "*result.second = ");
	log_to_console("\n");

	LOG("/**");
	LOG(" *  std::not_equal_to");
	LOG(" *  std::not_equal_to(a, b) <=> a != b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 3;);
	TRACE(b = 6;);
	res = std::not_equal_to<int>()(a,b);
	log_to_console((bool)res, "std::not_equal_to<int>()(a,b) returns: ");
	log_to_console("\n");


	LOG("/**");
	LOG(" * Using std::not_equal_to functor with the STL algorithm std::mismatch.");
	LOG(" * std::mismatch compares the two ranges with the help of std::not_equal_to.");
	LOG(" * The function returns a pair of iterators to the first element");
	LOG(" * in each range for which std::not_equal_to returns false.");
	LOG(" */");
	LOG("\n");

	result = std::mismatch(array1, array1 + arraySize1,
			array2, std::not_equal_to<int>());

	log_to_console("result = std::mismatch(array1, array1 + arraySize1,  array2, std::not_equal_to<int>());	//result = " );
	log_to_console(*result.first, "*result.first = ");
	log_to_console(*result.second, "*result.second = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::greater");
	LOG(" *  std::greater(a, b) <=> a > b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 33;);
	TRACE(b = 2;);
	res = std::greater<int>()(a,b);
	log_to_console((bool)res, "std::greater<int>()(a,b) returns: ");
	log_to_console("\n");

	LOG("/**");
	LOG(" * Using std::greater functor with the STL algorithm std::mismatch.");
	LOG(" * std::mismatch compares the two ranges with the help of std::greater.");
	LOG(" * The function returns a pair of iterators to the first element");
	LOG(" * in each range for which std::greater returns false.");
	LOG(" */");
	LOG("\n");

	int g1[] = { 100, 200, 5 };
	int g2[] = { 11, 22, 500 };
	int g1Size = sizeof(g1)/sizeof(g1[0]);
	result = std::mismatch(g1, g1 + g1Size,	g2, std::greater<int>());

	log_to_console(g1, g1Size, "g1 contains: ");
	log_to_console(g2, g1Size, "g2 contains: ");
	log_to_console("result = std::mismatch(g1, g1 + g1Size,	g2, std::greater<int>());");
	log_to_console(*result.first, "*result.first = ");
	log_to_console(*result.second, "*result.second = ");


	LOG("\n");
	LOG("/**");
	LOG(" *  std::less");
	LOG(" *  std::less(a, b) <=> a < b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 2;);
	TRACE(b = 99;);
	res = std::less<int>()(a,b);
	log_to_console((bool)res, "std::less<int>()(a,b) returns: ");

	LOG("\n");
	LOG("/**");
	LOG(" * Using std::less functor with the STL algorithm std::mismatch.");
	LOG(" * std::mismatch compares the two ranges with the help of std::less.");
	LOG(" * The function returns a pair of iterators to the first element");
	LOG(" * in each range for which std::less returns false.");
	LOG(" */");
	LOG("\n");

	int arr1[] = { 1, 2, 4, 120, 1001};
	int arr2[] = { 10, 20, 40 , 80, 100 };
	log_to_console(arr1, arraySize1, "arr1 contains: ");
	log_to_console(arr2, arraySize1, "arr2 contains: ");

	result = std::mismatch(arr1, arr1 + arraySize1, arr2, std::less<int>());

	log_to_console("result = std::mismatch(arr1, arr1 + arraySize1, arr2, std::less<int>());");
	log_to_console(*result.first, "*result.first = ");
	log_to_console(*result.second, "*result.second = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::greater_equal");
	LOG(" *  std::greater(a, b) <=> a >= b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 66;);
	TRACE(b = 10;);
	res = std::greater_equal<int>()(a,b);
	log_to_console((bool)res, "std::greater_equal<int>()(a,b) returns: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::less_equal");
	LOG(" *  std::greater(a, b) <=> a <= b");
	LOG(" */");
	LOG("\n");

	TRACE(a = 9;);
	TRACE(b = 55;);
	res = std::less_equal<int>()(a,b);
	log_to_console((bool)res, "std::less_equal<int>()(a,b) returns: ");
}

void logical_operations()
{
	LOG("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	LOG("			logical operations								  		");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	TRACE(bool a = true; );
	TRACE(bool b = false; );

	LOG("\n");
	LOG("/**");
	LOG(" *  std::logical_and: defines \"and\" logical operation (&&)");
	LOG(" *  std::logical_and(a, b) <=> a && b");
	LOG(" */");
	LOG("\n");

	bool res = std::logical_and<int>()(a, b);
	log_to_console((bool)res, "std::logical_and<bool>()(a, b) returns: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::logical_or: defines \"or\" logical operation (||)");
	LOG(" *  std::logical_and(a, b) <=> a || b");
	LOG(" */");
	LOG("\n");

	res = std::logical_or<int>()(a, b);
	log_to_console((bool)res, "std::logical_or<int>()(a, b) returns: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  std::logical_not: defines \"not\" logical operation (!)");
	LOG(" *  std::logical_not(a) <=> !a");
	LOG(" */");
	LOG("\n");

	res = std::logical_not<bool>()(a);
	log_to_console((bool)res, "std::logical_not<bool>()(a) returns: ");
}

}//namespace

/**
* Example using the STL functors
*/
void STLMoblet::STL_functors()
{
	LOG("\n");
	LOG("========================= STL functors ==========================================================================");
	LOG("/**");
	LOG("* STL provides definitions for several functors that are very useful in combination");
	LOG("* with the STL algorithms.");
	LOG("*");
	LOG("* STL functors define:");
	LOG("*");
	LOG("* 		- arithmetic operations: plus ( binary function <=> arg1 +  arg2 )");
	LOG("*					 minus ( binary function <=> arg1 -  arg2 )");
	LOG("*					 multiplies ( binary function <=> arg1 *  arg2 )");
	LOG("*					 divides ( binary function <=> arg1 /  arg2 )");
	LOG("*					 moduluS ( binary function <=> arg1 %  arg2 )");
	LOG("*					 negate ( unary function <=> -arg )");
	LOG("*");
	LOG("* 		- comparison operations: equal_to ( binary predicate <=> arg1 ==  arg2 )");
	LOG("*					 not_equal_to ( binary predicate <=> arg1 !=  arg2 )");
	LOG("*					 greater ( binary predicate <=> arg1 >  arg2  )");
	LOG("*					 less ( binary predicate <=> arg1 <  arg2  )");
	LOG("*					 greater_equal ( binary predicate <=> arg1 >=  arg2 )");
	LOG("*					 less_equal ( binary predicate <=> arg1 <=  arg2 )");
	LOG("*");
	LOG("* 		- logical operations:   logical_and ( binary predicate <=> arg1 &&  arg2 )");
	LOG("*					logical_or ( binary predicate <=> arg1 ||  arg2 )");
	LOG("*					logical_not ( unary predicate <=> !arg )");
	LOG("*");
	LOG("* Binary function means a function that takes with two arguments.");
	LOG("* Binary predicate means a function taking two arguments and returning a bool (see AboutPredicates.cpp).");
	LOG("*");
	LOG("* STL functors are defined in the <numeric> header.");
	LOG("*/");
	LOG("\n");

	LOG("				Example using the STL functors 						  ");



	arithmetic_functors();
	comparison_functors();
	logical_operations();

	LOG("\n");
}
