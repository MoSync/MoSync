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
#include "../TestSTL.h"
#include "../LOG.h"
#include "../DummyClass.h"


//
bool someUnaryFunction(const DummyClass &lv, const DummyClass &rv)
{
	return lv.getName() < rv.getName();
}

/**
 * Function for testing auxiliary functions from the <algorithm> header: min, max, swap
 * min, max template functions: If both values are equal, usually the first element is returned
 * 		Both functions are provided with an additional argument, that is the comparison criterion.
 * 		The default comparison criterion is operator<.
 * swap: provided to swap the values of two objects.
 * 		(The swap is possible only if the copy constructor and the assignment operators are defined.)
 */
void TestSTL::test_aux_functions()
{
	int small = 10;
	int big = 20;

	int resMin = std::min(small, big);
	TESTIFY_ASSERT(resMin == small);

	int resMax = std::max(small, big);
	TESTIFY_ASSERT(resMax == big);

	DummyClass smallDummy("AA");
	DummyClass bigDummy("BB"); //:)

	DummyClass minD = std::min(smallDummy, bigDummy, someUnaryFunction);
	TESTIFY_ASSERT(minD == smallDummy);

	DummyClass maxD = std::max(smallDummy, bigDummy, someUnaryFunction);
	TESTIFY_ASSERT(maxD == bigDummy);

	std::swap(small, big);
	resMin = std::min(small, big);
	TESTIFY_ASSERT(resMin == big);

	std::swap(smallDummy, bigDummy);
	minD = std::min(smallDummy, bigDummy, someUnaryFunction);
	TESTIFY_ASSERT(minD == bigDummy);

}

/**
 * Function for testing comparison operators defined in <utility>: !=, >, <=, >=
 * The operators are defined as template functions in std::rel_ops namespace
 */
void TestSTL::test_comparasion_operators()
{
	using namespace std::rel_ops;

	int small = -1;
	int big = 20;

	TESTIFY_ASSERT( small != big );

	TESTIFY_ASSERT( big > small );

	TESTIFY_ASSERT( small <= big );

	TESTIFY_ASSERT( big>=small );
}
