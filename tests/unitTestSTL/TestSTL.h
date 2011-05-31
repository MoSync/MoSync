/*
 * TestSTL.h
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */

#ifndef TESTSTL_H_
#define TESTSTL_H_

#include <testify/testify.hpp>

class TestSTL: public Testify::TestCase
{
public:
	TestSTL();

	/**
	 * Function for testing std::pair
	 * The pair structure is provided
	 * to treat two values as a single unit
	 * The pair structure is defined in <utility> header of STL
	 */
	void testPair();


	/**
	 * Function for testing std::auto_ptr
	 * auto_ptr is a smart pointer that takes ownership of the pointer it stores.
	 * When the auto_ptr object is destroyed, the pointer will be deleted.
	 */
	void test_auto_ptr();

	/**
	 * Function for testing function templates min and max
	 * If both values are equal, usually the first element is returned.
	 * Both functions are provided with an additional argument, that is the comparison criterion
	 * Those functions are defined in <algorithm>
	 */
	void test_aux_functions();

	/**
	 * Function for testing comparison operators defined in <utility>: !=, >, <=, >=
	 * The operators are defined as template functions in std::rel_ops namespace
	 */
	void test_comparasion_operators();

	/**
	 * Function for testing the STL vector container. Vector is a sequence container, that is
	 * the position of an element depends on the time and place of the insertion, and not of
	 * the value it has.
	 * std::vector keeps its elements in a dynamic array.  It has random access. Has the fastest
	 * access speed from all STL containers. Also has the fastest iterators from all STL containers.
	 * Appending and removing elements at the end of the container is very fast. Inserting an element
	 * at the middle or at the beginning, means that all the following elements have to be moved to make room
	 * for the new element, so it can take time.
	 * std::vector is guaranteed to have the lowest space overhead pe contained object
	 * from all the STL containers(zero bytes).
	 * Is the only STL container that guarantees that two object near each other in container,
	 * will be near each other in memory.
	 */
	void test_vector();

	/**
	 *  test function for STL container std::list
	 *  std::list is implemented as a doubly linked list of elements
	 *  std::list doesn't have random access. For accessing a element, you have to
	 *  iterate through list, until you reach the element. Compared with std::vector or std::deque
	 *  is not efficient, when accessing an random element.
	 *  std::list is efficient for insertion and removal of elements anywhere in the container.
	 *  Also for moving elements and block of elements within the container.
	 *	The std::list is efficient in removing or inserting an element in any position.
	 *	(From this point of view is more efficient than vector or deque.)
	 *	std::list is defined in <list> header
	 */
	void test_list();

	/**
	 * Function for testing the STL deque container.
	 * std::deque doesn't keep it's elements in a single block of memory (like vector). It uses multiple
	 * blocks of memory and keeps track of them. Because of that is fast on insertions at the end and also at the
	 * beginning. Also it doesn't need to copy and destroy objects when it needs to allocate more memory.
	 * Provides random access to its elements.
	 * std::deque is defined in the <deque> header.
	 */
	void test_deque();
};

#endif /* TESTSTL_H_ */
