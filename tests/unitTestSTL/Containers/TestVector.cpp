/*
 * TestVector.cpp
 *
 *  Created on: May 30, 2011
 *      Author: gabi
 */


#include <vector>
#include "../TestSTL.h"


/**
 * Function for testing the STL vector container.
 * std::vector keeps its elements in a dynamic array and maintains its storage as a single contiguous
 * array of objects (in order to have efficient indexing and iteration). A dynamic array is allocated
 * first time the vector is constructed. When vector needs a bigger array, to hold its elements,
 * it will allocate a new, bigger, chunk of memory and will copy all the elements to this new chunk, deleting the
 * old chunk and destroying the objects contained in it.
 * std::vector provides random access to its elements.
 * Has the fastest access speed of all STL containers. Also has the fastest iterators.
 * Appending and removing elements at the end of the container is very fast. Inserting an element
 * at the middle or at the beginning, means that all the following elements have to be moved to make room
 * for the new element, so it can take time.
 * std::vector is guaranteed to have the lowest space overhead per contained object
 * of all the STL containers(zero bytes).
 * It is the only STL container that guarantees that two object near each other in container,
 * will be near each other in memory.
 * The vector container is defined in <vector> header.
 */

#include <vector>
#include "../LOG.h"

void TestSTL::test_vector()
{
	using std::vector;

	/**
	 * default constructor
	 */
	vector<int> ve;

	/**
	* empty function: returns true if the vector contains no elements
    */
	TESTIFY_ASSERT(ve.empty() == true);

	/**
	 * constructor
	 * fills the vector with 10 elements (uninitialized ints).
	 */
	vector<int> v0(10);

	/**
	 * constructor
	 * fills the v1 vector with 20 ints (initialized with -1).
	 */
	vector<int> v1(20, -1);
	TESTIFY_ASSERT(v1.size() == 20);

	/**
	 * constructor
	 * constructs a vector from an array
	 */
	int array[]= {1, 2, 3, 4, 5};
	int numElements = sizeof(array)/sizeof(array[0]);
	vector<int> v2(array, array + numElements);
	TESTIFY_ASSERT(v2.size() == numElements);

	/**
	 * copy constructor
	 */
	vector<int> v2_copy(v2);

	/**
	 * operator ==
	 */
	TESTIFY_ASSERT( v2 == v2_copy);

	/**
	 * assignment operator
	 */
	vector<int> v3;
	v3 = v2;
	TESTIFY_ASSERT(v3 == v2);

	/**
	 * assign function
	 * copies all the elements from d3 to d4.
	 */
	vector<int> v4;
	v4.assign(v3.begin(), v3.end());
	TESTIFY_ASSERT( v4 == v3 );

	/**
	 * clear function: all the elements in the vector are destroyed and removed from the vector,
	 * after calling this function the vector will have the size zero.
	 */
	v4.clear();
	TESTIFY_ASSERT(v4.size() == 0);

	/**
	 * overload version of the assign function
	 * copies only the first 2 elements from v3 to v4
	 */
	v4.assign(v3.begin(), v3.begin() + 2);
	TESTIFY_ASSERT(v4.size() == 2);

	/**
	 * resize function: resizes the vector to contain the specified number of elements
	 * prototype: void resize(size_t newSize, T obj = T())
	 * if the specified size is smaller then the current size,
	 * the content is reduced to the first "newSize" elements, and the rest are destroyed.
	 * If the "newSize" is bigger than the current size, the vector is expanded by adding
	 * at the end the number of objects needed. The objects are created by copy constructing after "c".
	 * resize doesn't affect the capacity of the vector
	 */
	v4.resize(1);
	TESTIFY_ASSERT(v4.size() == 1);

	/**
	 * overload version of the assign function
	 * fills the vector with 10 elements, having the value "-6"
	 */

	v4.assign(10, -6);
	TESTIFY_ASSERT( v4.size() == 10);

	/**
	 * push_back: adds an element at the end of the vector
	 */
	v4.push_back(99);
	TESTIFY_ASSERT( v4.size() == 11 );

	/**
	 * pop_back: removes and destroys the last element of the vector (by resizing it by one)
	 */
	v4.pop_back();
	TESTIFY_ASSERT( v4.size() == 10 );

	/**
	 * begin: returns an iterator refering to the first element of the vector
	 */
	std::vector<int>::iterator itBegin = v4.begin();

	/**
	 * insert function: inserts an element before the specified position.
	 * The position before it should insert the element is indicated by an iterator
	 * inserts the value "1001", before the first element. The first element will be 1001,
	 * after this function call.
	 */
	v4.insert(itBegin, 1001);
	TESTIFY_ASSERT(*itBegin == 1001);

	/**
	 * inserts before the second element the first 3 elements from v1.
	 */
	v4.insert(itBegin + 1, v1.begin(), v1.begin() + 3);

	/**
	 * inserts 7 elements, with the value -90, before the first element of v4.
	 */
	v4.insert(itBegin, 7, -90);
	itBegin = v4.begin();
	TESTIFY_ASSERT(*itBegin == -90);

	/**
	 *  erase function: removes a single element, or a range of elements
	 */
	v4.erase(itBegin, itBegin + 1);

	/**
	 * operator[]: returns a reference to the element at the specified position.
	 */
	int x = v4[0];

	/**
	 * at: returns a reference to the element at the spefified position.
	 * same as operator[], with the difference that "at" function will
	 * throw a out_of_range exception if the requested index is out of range.
	 */
	int x2 = v4.at(0);

	/**
	 * end: returns an iterator referring to the one past the end element in the vector
	 */
	std::vector<int>::iterator iter;
	for(iter = itBegin; iter != v4.end(); ++iter)
	{
		//adds 20 to each element of vector
		*iter += 20;
	}

	/**
	 * front: returns a reference to the first element
	 */
	int &a = v4.front();
	a = 88;
	TESTIFY_ASSERT(v4.front() == 88);

	/**
	 * back: returns a reference to the last element
	 */
	int &b = v4.back();
	b = -77;
	TESTIFY_ASSERT( v4.back() == -77);
}
