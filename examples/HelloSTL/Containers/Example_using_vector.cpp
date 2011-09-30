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
 * @file TestVector.cpp
 * @author Gabriela Rata
 */

#include <vector>
#include <string>
#include <assert.h>
#include "../STLMoblet.h"


/**
 * std::vector keeps its elements in a dynamic array and maintains its storage
 * as a single contiguous
 * array of objects (in order to have efficient indexing and iteration).
 * A dynamic array is allocated first time the vector is constructed. When
 * vector needs a bigger array, to hold its elements,
 * it will allocate a new, bigger, chunk of memory and will copy all the
 * elements to this new chunk, deleting the old chunk and destroying the
 * objects contained in it.
 * std::vector provides (the fastest) random access to its elements. It has a
 * random access iterator. That means that we can iterate through a vector step
 * by step, using the ++ operator and --operator. And also we can move the
 * iterator more then a step, and have access to random positions like this:
 * 			++myVectorIterator; 	//ok. Move one step forward
 * 			--myVectorIterator; 	//ok. Move one step backward
 * 			myVectorIterator + 5;	//ok. Move the iterator 5 positions forward.
 * 			myVectorIterator - 2 	//ok
 * Has the fastest access speed of all STL containers. Also has the fastest
 * iterators.
 * Appending and removing elements at the end of the container is very fast.
 * Inserting an element at the middle or at the beginning, means that all the
 * following elements have to be moved to make room for the new element, so it
 * can take time.
 * std::vector is guaranteed to have the lowest space overhead per contained
 * object of all the STL containers(zero bytes).
 * It is the only STL container that guarantees that two object near each other
 * in container, will be near each other in memory.
 * The vector container is defined in <vector> header.
 */

#include <vector>

#include "../STLMoblet.h"
#include "../LOG.h"

void STLMoblet::STL_vector()
{
	LOG("\n");

	LOG("========================= example using vector ===================================================================");

	using std::vector;

	/**
	 * default constructor
	 */
	vector<int> ve;

	/**
	* empty function: returns true if the vector contains no elements
    */
	log_to_console(ve.empty(), std::string("ve.empty() returns "));

	/**
	 * constructor
	 * fills the vector with 3 elements (uninitialized ints).
	 */
	vector<int> v0(3);
	log_to_console(v0, "v0 contains: ");

	/**
	 * constructor
	 * fills the v1 vector with 3 ints (initialized with -1).
	 */
	vector<int> v1(3, -1);
	log_to_console(v1, "v1 contains: ");

	/**
	 * constructor
	 * constructs a vector from an array
	 */
	int array[]= {1, 2, 3, 4, 5};
	int numElements = sizeof(array)/sizeof(array[0]);
	vector<int> v2(array, array + numElements);
	log_to_console(v2, "v2 contains: ");


	/**
	 * copy constructor
	 */
	vector<int> v2_copy(v2);
	log_to_console(v2_copy, "v2_copy contains: ");

	/**
	 * operator ==
	 */
	assert(v2 == v2_copy);

	/**
	 * assignment operator
	 */
	vector<int> v3;
	v3 = v2;
	log_to_console(v3, "v3 contains: ");

	/**
	 * assign function
	 * copies all the elements from v3 to v4.
	 */
	vector<int> v4;
	v4.assign(v3.begin(), v3.end());
	log_to_console(v4, "v4 contains, after calling v4.assign(v3.begin(),"
			" v3.end()): ");


	/**
	 * clear function: all the elements in the vector are destroyed and removed
	 * from the vector,
	 * after calling this function the vector will have the size zero.
	 */
	v4.clear();
	log_to_console(v4, "v4 contains, after calling v4.clear(): ");

	/**
	 * overload version of the assign function
	 * copies only the first 2 elements from v3 to v4
	 */
	v4.assign(v3.begin(), v3.begin() + 2);
	log_to_console(v4, "v4 contains, after calling v4.assign(v3.begin(),"
			" v3.begin() + 2): ");

	/**
	 * resize function: resizes the vector to contain the specified number of
	 * elements
	 * prototype: void resize(size_t newSize, T obj = T())
	 * if the specified size is smaller then the current size,
	 * the content is reduced to the first "newSize" elements, and the rest are
	 * destroyed.
	 * If the "newSize" is bigger than the current size, the vector is expanded
	 * by adding
	 * at the end the number of objects needed. The objects are created by copy
	 * constructing after "c".
	 * resize doesn't affect the capacity of the vector
	 */
	v4.resize(1);
	log_to_console(v4, "v4 contains, after calling v4.resize(1): ");

	/**
	 * overload version of the assign function
	 * fills the vector with 3 elements, having the value "-6"
	 */
	v4.assign(1, -6);
	log_to_console(v4, "v4 contains, after calling v4.assign(1, -6): ");


	/**
	 * push_back: adds an element at the end of the vector
	 */
	v4.push_back(99);
	log_to_console(v4, "v4, after calling v4.push_back(99): ");

	/**
	 * pop_back: removes and destroys the last element of the vector
	 * (by resizing it by one)
	 */
	v4.pop_back();
	log_to_console(v4, "v4, after calling v4.pop_back(99): ");

	/**
	 * begin: returns an iterator referring to the first element of the vector
	 * If the container is empty, it will return the one past-the-end element
	 * in the container,
	 * like the set::end() function. See bellow.
	 */
	std::vector<int>::iterator itBegin = v4.begin();
	log_to_console(*itBegin, "std::vector<int>::iterator itBegin = v4.begin(); "
			"*itBegin = ");

	/**
	 * end: returns an iterator referring to the one past the end element in
	 * the vector
	 * We must not deference the iterator returned by vector::end(). It is used
	 * to see is if we reached the
	 * end of the container, when we iterate through it.
	 */
	std::vector<int>::iterator itEnd = v4.end();
	if( v4.empty() )
	{
		assert(itBegin == itEnd);
	}
	else
	{
		//the v4 vector is not empty, so v4.begin() !=  v4.end()
		assert(itBegin != itEnd);
	}
	std::vector<int>::iterator iter;
	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//adds 20 to each element of vector
		*iter += 20;
	}
	log_to_console(v4, "iterate through v4 and add 20, to each element. "
			"v4 contains now: ");


	/**
	 * insert function: inserts an element before the specified position.
	 * The position before it should insert the element is indicated by an
	 * iterator inserts the value "1001", before the first element. The first
	 * element will be 1001, after this function call.
	 */
	v4.insert(itBegin, 1001);
	log_to_console(v4, "v4 after calling v4.insert(itBegin, 1001): ");

	/**
	 * inserts before the second element the first 3 elements from v1.
	 */
	v4.insert(itBegin + 1, v1.begin(), v1.begin() + 3);
	log_to_console(v4, "v4 after calling v4.insert(itBegin + 1, v1.begin(), v1.begin() + 3): ");

	/**
	 * inserts 2 elements, with the value -90, before the first element of v4.
	 */
	v4.insert(itBegin, 2, -90);
	itBegin = v4.begin();

	log_to_console(v4, "calling v4.insert(itBegin, 2, -90); v4 contains: ");
	log_to_console(*itBegin, "\t\t\t\t\t*itBegin = ");

	/**
	 *  erase function: removes a single element, or a range of elements
	 */
	v4.erase(itBegin, itBegin + 1);
	log_to_console(v4, "v4 after calling v4.erase(itBegin, itBegin + 1): ");

	/**
	 * operator[]: returns a reference to the element at the specified position.
	 */
	int x = v4[0];
	log_to_console(v4[0], "calling operator[], v4[0] returns ");

	/**
	 * at: returns a reference to the element at the spefified position.
	 * same as operator[], with the difference that "at" function will
	 * throw a out_of_range exception if the requested index is out of range.
	 */
	int x2 = v4.at(0);
	log_to_console(v4[0], "v4.at(0) returns");

	/**
	 * front: returns a reference to the first element
	 */
	int &a = v4.front();
	log_to_console(a, "v4.front() returns ");

	a = 88;
	assert(v4.front() == 88);

	/**
	 * back: returns a reference to the last element
	 */
	int &b = v4.back();
	log_to_console(b, "v4.back() returns ");
	b = -77;
	assert( v4.back() == -77);

	LOG("\n");
}
