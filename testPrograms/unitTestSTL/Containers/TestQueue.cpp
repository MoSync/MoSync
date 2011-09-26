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
 * @file TestQueue.cpp
 * @author Gabriela Rata
 */

#include <queue>
#include "../TestSTL.h"
#include "../DummyClass.h"

/**
 *  std::queue is designed to operate in a FIFO mode (first-in first-out).
 *  std::priority_queue is implemented as a container adaptor. Containers adapters are classes that use an
 *  encapsulated container and provide a restricted interface to that container.
 *  The underlying container can be a STL container or some other  container type. It has to provide the
 *  following public member functions: front(), back(), push_back(), pop_front().
 *  priority_queue is defined as a template taking three parameters:
 *  		template < 	class T,
 *  					class Container = deque<T>
 *  				>
 *  				class queue;
 * 		The first parameter is the type of elements it will contain.
 * 		The second parameter is the underlying container, and is by default std::deque.
 * std::priority_queue is defined in the <queue> header.
 */

void TestSTL::test_queue()
{
	using std::queue;

	/**
	*  default constructor
	*  Constructed with a std::vector as the underlying container and std::less as the comparison class.
	*/
	queue<int> pq;

	/**
	* empty function: returns true if the priority_queue contains no elements
	*/
	TESTIFY_ASSERT(pq.empty() == true);

	/**
	* default constructor
	* Constructed with a std::vector as the underlying container.
	*/
	queue<DummyClass> pq1;

	/**
	 * push: Inserts a new element into the priority_queue. The new element is sorted into the queue according
	 * to a comparison functor.
	 * push calls the member function push_back of the underlying container object ( std::vector in our case ).
	 */
	DummyClass d1("first");
	DummyClass d2("second");
	DummyClass d3("third");
	DummyClass d4("fourth");
	DummyClass d5("fifth");
	pq1.push(d1);
	pq1.push(d2);
	pq1.push(d3);
	pq1.push(d4);
	pq1.push(d5);

	/**
	* size function: returns the number of elements in the queue
	*/
	TESTIFY_ASSERT(pq1.size() == 5);

	/**
	* front function: returns a reference to the "oldest" element in the queue.
	*/
	TESTIFY_ASSERT(pq1.front() == d1);		//d1 is the first element pushed into pq1

	/**
	* back function: returns a reference to the "newest" element in the queue (last element pushed into the queue).
	*/
	TESTIFY_ASSERT(pq1.back() == d5);		//d5 is the last element pushed into pq1

	/**
	* pop function: removes the "oldest" element from the queue.
	* queue::pop calls the member function pop_front of the underlying container object.
	*/
	pq1.pop();								//remove d1
	TESTIFY_ASSERT(pq1.front() == d2);
	pq1.pop();								//remove d2
	TESTIFY_ASSERT(pq1.front() == d3);
	pq1.pop();								//remove d3
	TESTIFY_ASSERT(pq1.front() == d4);
	pq1.pop();								//remove d4
	TESTIFY_ASSERT(pq1.front() == d5);
	pq1.pop();								//remove d5
	TESTIFY_ASSERT(pq1.empty() == true);
}
