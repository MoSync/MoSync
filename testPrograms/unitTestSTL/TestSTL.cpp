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
 * @file TestSTL.cpp
 * @author Gabriela Rata
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

		addTest(
				bind(&TestSTL::example_iterators, this), "example_iterators");

		//*******************************************************************************************
		//						STRING
		//*******************************************************************************************
		addTest(
				bind(&TestSTL::test_string, this), "test_string");

		//cout<<"x";

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
