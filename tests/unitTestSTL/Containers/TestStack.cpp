/*
 * TestStack.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */

#include <stack>
#include <vector>
#include "../TestSTL.h"

/**
 *  stack is a container that operates as in a LIFO (last in first out) mode.
 *  The elements are inserted and extracted only from the end of the container.
 *  std::stack is implemented as a container adaptor. Containers adapters are classes that use an
 *  encapsulated container and provide a restricted interface to that container.
 *  The underlying container can be a STL container or some other  container type. It has to provide the
 *  following public member functions: back(), push_back(), pop_back().
 *  stack is defined as a template taking with two parameters:
 *  		template < 	class T,
 *  					class Container = deque<T>
 *  				>
 *  				class stack;
 * 		The first parameter is the type of elements in will contain.
 * 		The second parameter is the underlying container, and is by default std::deque.
 * std::stack is defined in the <stack> header.
 */
void TestSTL::test_stack()
{
	using std::stack;

	/**
	 * default constructor
	 * Constructed with a std::deque as the underlying container. That means that it will forward all
	 * the function calls to it's deque.
	 */
	stack<int> s;

	/**
	* empty function: returns true if the stack contains no elements.
	* It calls the member function deque::empty
    */
	TESTIFY_ASSERT(s.empty() == true);

	/**
	 * push: adds an element at the end of the underlying deque. Calls deque::push_back.
	 */
	s.push(10);

	/**
	 * constructor
	 * s1 is constructed with a vector as its underlying container. That means that it will forward all
	 * the function calls to this vector.
	 */
	std::vector<int> underlyingContainer;
	stack<int, std::vector<int> > s1(underlyingContainer);

	/**
	 * size: returns the number of elements from the underlying vector (calls vector::size()).
	 */
	TESTIFY_ASSERT(s1.size() == 0);

	/**
	 * top function: returns the last element added to the stack.
	 * In the example bellow calls vector::back().
	 */
	s1.push(32);
	TESTIFY_ASSERT(s1.top() == 32);

	/**
	 * pop function: Removes the element on top of the stack.
	 * In the example bellow, it will call vector::pop_back;
	 */
	s1.pop();
	TESTIFY_ASSERT(s1.empty() == true);
}
