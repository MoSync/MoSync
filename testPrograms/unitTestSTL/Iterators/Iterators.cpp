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
 * @file Iterators.cpp
 * @author Gabriela Rata
 */

#include <vector>
#include <set>
#include <list>
#include "../TestSTL.h"

/**
*  An iterator is an object that can iterate through a range, pointing to the elements in the range.
*  An iterator has to provide at least operator++, to iterate forward the range. Also it has to
*  provide the dereference operator(*), so that we can access the object
*  to which it points.
*  All containers define their own iterators, which have different abilities.
*  Depending on the the abilities they have, iterators are classified in five categories:
*  		-> Input iterators
*  		-> Output iterators
*  		-> Forward iterators
*  		-> Bidirectional iterators
*  		-> Random access iterators.
*/

void TestSTL::example_iterators()
{
	/**
	*  Input iterators: allow only reading elements from a sequence and moving forward, one step,
	*  using operator++. The elements are read with operator*.
	*  Provides also operator== and operator!=.
	*  An input iterator implemented in STL is for example the one provided by std::istream.
	*/

	/**
	*  Output iterators: allow only writing elements from a sequence and only moving forward, one step,
	*  using operator++. They elements are written with operator*.
	*  Provides also operator== and operator!=.
	*  An input iterator implemented in STL is for example the one provided by std::ostream.
	*/

	/**
	*  Forward iterators: allow reading and writing elements from a sequence. Only moving forward, one step,
	*  using operator++ is possible. They elements are read and written with operator*.
	*  Provides also operator== and operator!=.
	*  An input iterator implemented in STL is for example the one provided by std::ostream.
	*/

	/**
	*  Bidirectional iterators: allow reading and writing elements from a sequence. Allows moving forward and backward,
	*  one step,using operator++/operator--. Their elements are read and written with operator*.
	*  Provides also operator== and operator!=.
	*  An input iterator implemented in STL is for example the ones provided by list, multiset, map, multimap.
	*/

	int sArray[] = { 0, 11, 22, 33, 44 };
	int sArraySize = sizeof(sArray)/sizeof(sArray[0]);
	std::list<int> myList(sArray, sArray + sArraySize);

	//create an bidirectional iterator
	std::list<int>::iterator iter;
	iter = myList.begin();
	//read
	TESTIFY_ASSERT( 0 == *iter );

	//write
	*iter = 99;
//
//	operator++
	iter++;
    TESTIFY_ASSERT( 11 ==  *iter );

//    operator--
    --iter;
    TESTIFY_ASSERT( 99 ==  *iter );

    /**
	*  Random access iterators: allows all operations a normal pointer does: add and subtract integral values,
	*  move forward and backward, one or more steps, use operator[] on it, subtract one iterator from another.
	*  It overloads operator<, operator>, operator==, operator!=.
	*  An input iterator implemented in STL is for example the ones provided by vector, deque, string.
	*/

    std::vector<int> myVector(sArray, sArray + sArraySize);	//myVector contains now: 0, 11, 22, 33, 44

    //create an random iterator
	std::vector<int>::iterator randomAcessIter;
	randomAcessIter = myVector.begin();

	//read
	TESTIFY_ASSERT( 0 == *randomAcessIter );

	//write
	*randomAcessIter = 99;										//myVector contains now: 99, 11, 22, 33, 44

	//operator++
	randomAcessIter++;
	TESTIFY_ASSERT( 11 ==  *randomAcessIter );

	//operator--
	--randomAcessIter;
	TESTIFY_ASSERT( 99 ==  *randomAcessIter );

	//move two steps forward
	randomAcessIter += 2;
	TESTIFY_ASSERT( 22 ==  *randomAcessIter );

	std::vector<int>::iterator start = myVector.begin();
	TESTIFY_ASSERT( start != randomAcessIter );

	//operator <
	for( std::vector<int>::iterator it = start; it < (myVector.end() - 1); ++it)
	{
		*it = *(it + 1);
	}
	TESTIFY_ASSERT( 11 == myVector[0] &&
					22 == myVector[1] &&
					33 == myVector[2] &&
					44 == myVector[3]);
}
