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
