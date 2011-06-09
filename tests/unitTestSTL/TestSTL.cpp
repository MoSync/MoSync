/*
 * TestSTL.cpp
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */


#include "TestSTL.h"

TestSTL::TestSTL() : TestCase("STLTest")
{
		//*******************************************************************************************
		//						UTILITIES: pair, auto_ptr, max, min, !=, >, <=, >=
		//*******************************************************************************************

		addTest(
			bind(&TestSTL::testPair, this),	"testPair");

		addTest(
			bind(&TestSTL::test_auto_ptr, this), "test_auto_ptr");

		addTest(
			bind(&TestSTL::test_aux_functions, this), "test_min_and_max_functions");

		addTest(
			bind(&TestSTL::test_comparasion_operators, this), "testComparasionOperators");

		//*******************************************************************************************
		//						CONTAINERS
		//*******************************************************************************************

		addTest(
			bind(&TestSTL::test_vector, this), "test_vector");

		addTest(
			bind(&TestSTL::test_list, this), "test_list");

		addTest(
			bind(&TestSTL::test_deque, this), "test_deque");

		addTest(
			bind(&TestSTL::test_set, this), "test_set");

		addTest(
			bind(&TestSTL::test_multiset, this), "test_multiset");

		addTest(
			bind(&TestSTL::test_stack, this), "test_stack");

		addTest(
			bind(&TestSTL::test_prority_queue, this), "test_priority_queue");

		addTest(
				bind(&TestSTL::test_queue, this), "test_queue");

		addTest(
				bind(&TestSTL::test_bitset, this), "test_bitset");

		//*******************************************************************************************
		//						ALGORITHMS
		//*******************************************************************************************

		addTest(
				bind(&TestSTL::predicate_example, this), "predicate_example");

		addTest(
				bind(&TestSTL::example_functors, this), "example_functors");

		addTest(
				bind(&TestSTL::test_functors, this), "test_functors");

		addTest(
				bind(&TestSTL::test_algorithms, this), "test_algorithms");

		addTest(
				bind(&TestSTL::test_adaptors, this), "test_adaptors");

		//*******************************************************************************************
		//						STRING
		//*******************************************************************************************
		addTest(
				bind(&TestSTL::test_string, this), "test_string");

}


namespace NumericUtilities
{

bool isEven(int x)
{
	return x%2 == 0;
}

bool isOdd(int x)
{
	return (x%2!=0);
}

bool isNegative(int x)
{
	return x < 0;
}

}//end namespace NumericUtilities

