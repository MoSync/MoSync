/*
 * TestSTL.cpp
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */


#include "TestSTL.h"

TestSTL::TestSTL() : TestCase("STLTest")
{
		addTest(
			bind(&TestSTL::testPair, this),	"testPair");

		addTest(
			bind(&TestSTL::test_auto_ptr, this), "test_auto_ptr");

		addTest(
			bind(&TestSTL::test_aux_functions, this), "test_min_and_max_functions");

		addTest(
			bind(&TestSTL::test_comparasion_operators, this), "testComparasionOperators");

		addTest(
				bind(&TestSTL::test_vector, this), "test_vector");

		addTest(
				bind(&TestSTL::test_list, this), "test_list");

		addTest(
				bind(&TestSTL::test_deque, this), "test_deque");
}
