/*
 * TestDeque.cpp
 *
 *  Created on: May 30, 2011
 *      Author: gabi
 */

#include <deque>
#include "../TestSTL.h"

/**
 * Function for testing the STL deque container.
 * std::deque doesn't keep it's elements in a single block of memory (like vector). It uses multiple
 * blocks of memory and keeps track of them. Because of that is fast on insertions at the end and also at the
 * beginning. Also it doesn't need to copy and destroy objects when it needs to allocate more memory.
 * Provides (reasonable fast) random access to its elements. It has a random access iterator. That means that we can iterate
 * through a vector step by step, using the ++ operator and --operator. Also we can move the iterator more then a step, and
 * have access to random positions like this:
 * 				++myVectorIterator; 	//ok. Move one step forward
 * 				--myVectorIterator; 	//ok. Move one step backward
 * 				myVectorIterator + 5; 	//ok. Move the iterator 5 positions forward.
 * 				myVectorIterator - 2	//ok
 * std::deque is defined in the <deque> header.
 */
void TestSTL::test_deque()
{
	using std::deque;

	/**
	 * default constructor
	 */
	deque<int> d;

	/**
	* empty function: returns true if the deque contains no elements
	*/
	TESTIFY_ASSERT(d.empty() == true);

	/**
	 * constructor
	 * fills the deque with 3 elements (uninitialized ints).
	 */
	deque<int> d0(3);

	/**
	 * constructor
	 * fills the deque vector with 20 ints (initialized with -1).
	 */
	deque<int> d1(20, -1);
	TESTIFY_ASSERT(d1.size() == 20);

	/**
	 * constructor
	 * constructs a vector from an array
	 */
	int array[]= {1, 2, 3, 4, 5};
	int numElements = sizeof(array)/sizeof(array[0]);
	deque<int> d2(array, array + numElements);
	TESTIFY_ASSERT(d2.size() == numElements);

	/**
	 * copy constructor
	 */
	deque<int> d2_copy(d2);

	/**
	 * operator ==
	 */
	TESTIFY_ASSERT( d2 == d2_copy);

	/**
	 * assignment operator
	 */
	deque<int> d3(40, -2);
	d3 = d2;
	TESTIFY_ASSERT(d3 == d2);

	/**
	 * assign function
	 * copies all the elements from d3 to d4.
	 */
	deque<int> d4;
	d4.assign(d3.begin(), d3.end());
	TESTIFY_ASSERT( d4 == d3 );

	/**
	 * clear function: all the elements in the vector are destroyed and removed from the vector,
	 * after calling this function the vector will have the size zero.
	 */
	d4.clear();
	TESTIFY_ASSERT(d4.size() == 0);

	/**
	 * overload version of the assign function
	 * copies only the first 3 elements from d3 to d4
	 */
	d4.assign(d3.begin(), d3.begin() + 3);
	TESTIFY_ASSERT(d4.size() == 3);

	/**
	 * resize function: resizes the vector to contain the specified number of elements
	 * prototype: void resize(size_t newSize, T obj = T())
	 * if the specified size is smaller then the current size,
	 * the content is reduced to the first "newSize" elements, and the rest are destroyed.
	 * If the "newSize" is bigger than the current size, the deque is expanded by adding
	 * at the end the number of objects needed. The objects are created by copy constructing after "c".
	 */
	d4.resize(9);
	TESTIFY_ASSERT(d4.size() == 9);

	/**
	 * overload version of the assign function
	 * fills the deque with 10 elements, having the value "-68"
	 */

	d4.assign(10, -68);
	TESTIFY_ASSERT( d4.size() == 10);

	/**
	 * push_back: adds an element at the end of the deque
	 */
	d4.push_back(99);
	TESTIFY_ASSERT( d4.size() == 11 );

	/**
	 * pop_back: removes and destroys the last element of the vector (by resizing it by one)
	 */
	d4.pop_back();
	TESTIFY_ASSERT( d4.size() == 10 );

	/**
	 * push_front: adds an element at the beginning of the deque
	 */
	d4.push_front(57);
	TESTIFY_ASSERT( d4.size() == 11 );

	/**
	 * pop_front: removes and destroys the first element of the deque
	 */
	d4.pop_front();
	TESTIFY_ASSERT( d4.size() == 10 );

	/**
	 * begin: returns an iterator refering to the first element of the deque
	 * If the container is empty, it will return the one past-the-end element in the container,
	 * like the set::end() function. See bellow.
	 */
	std::deque<int>::iterator itBegin = d4.begin();

	/**
	 * end: returns an iterator referring to the one past the end element in the deque
	 * We must not deference the iterator returned by end(). It is used to see is if we reached the
	 * end of the container, when we iterate through it.
	 */
	std::deque<int>::iterator itEnd = d4.end();
	if( d4.empty() )
	{
		TESTIFY_ASSERT(itBegin == itEnd);
	}
	else
	{
		//the d4 deque is not empty, so d4.begin() !=  d4.end()
		TESTIFY_ASSERT(itBegin != itEnd);
	}
	//iterate through d4
	std::deque<int>::iterator iter;
	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//adds 1 to each element of deque
		*iter += 1;
	}

	/**
	 * insert function: inserts an element before the specified position.
	 * The position before it should insert the element is indicated by an iterator
	 * inserts the value "1001", before the first element. The first element will be 1001,
	 * after this function call.
	 */
	d4.insert(itBegin, 1001);
	itBegin = d4.begin();
	TESTIFY_ASSERT(*itBegin == 1001);

	/**
	 * inserts before the second element the first 3 elements from d1.
	 */
	d4.insert(itBegin + 1, d1.begin(), d1.begin() + 3);

	/**
	 * inserts 7 elements, with the value -93, before the first element of d4.
	 */
	d4.insert(itBegin, 7, -93);
	itBegin = d4.begin();

	/**
	 *  erase function: removes a single element, or a range of elements
	 */
	d4.erase(itBegin, itBegin + 1);

	/**
	 * operator[]: returns a reference to the element at the specified position.
	 */
	int x = d4[0];

	/**
	 * at: returns a reference to the element at the specified position.
	 * same as operator[], with the difference that the "at" function will
	 * throw an out_of_range exception, if the requested index is out of range.
	 */
	int x2 = d4.at(0);

	/**
	 * front: returns a reference to the first element
	 */
	int &a = d4.front();
	a = 80;
	TESTIFY_ASSERT(d4.front() == 80);

	/**
	 * back: returns a reference to the last element
	 */
	int &b = d4.back();
	b = -77;
	TESTIFY_ASSERT( d4.back() == -77);
}
