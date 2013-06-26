/*
 * Pairs.cpp
 *
 *  Created on: May 26, 2011
 *      Author: gabi
 */

#include <string>
#include <cassert>
#include <utility>
#include "../TestSTL.h"

/**
 * Function for testing std::pair
 * The pair structure is provided
 * to treat two values as a single unit
 * The pair structure is defined in <utility> header of STL
 */
void TestSTL::testPair()
{
	using std::pair;

	/**
	 * default constructor
	 */
	pair<float, char> p1;

	/**
	 * initialize the  pair object
	 */
	p1.first = 0.09;
	p1.second = 'A';

	/**
	 * The template function creates a pair object
	 * without the user having to write the types explicitly
	 * copy constructor called
	 */
	pair<float, char> p2(std::make_pair(10.12, 'B'));

	/**
	 * assignment operator called
	 */
	p1 = p2;

	/**
	 * operator ==
	 * returns true if p1.first == p2.first && p1.second == p2.second
	 */
	TESTIFY_ASSERT(p1 == p2);

	/**
	 * operator !=
	 * implemented as !(operator==(arg1, arg2))
	 */
	p1.first -= 20;
	TESTIFY_ASSERT(p1 != p2 );

	/**
	 * operator <
	 * returns true if p1.first < p2.first && p1.second < p2.second
	 */
	TESTIFY_ASSERT(p1 < p2 );

	/**
	 * operator >
	 * returns true if p1.first > p2.first && p1.second > p2.second
	 */

	TESTIFY_ASSERT(p2 > p1);

	p1 = p2;

	/**
	 * operator <=
	 * implemented as (!(p2 < p1))
	 */
	TESTIFY_ASSERT( p1 <= p2);

	/**
	 * operator >=
	 * implemented as if (!(p1 < p2))
	 */
	TESTIFY_ASSERT( p1 >= p2);
}
