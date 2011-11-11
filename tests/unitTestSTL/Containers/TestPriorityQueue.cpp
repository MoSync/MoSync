/*
 * TestPriorityQueue.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */

/**
 *
 */

#include <queue>
#include <vector>
#include "../TestSTL.h"
#include "../DummyClass.h"
#include "../FunctorForDummyClass.h"
#include "../LOG.h"

/**
 *  std::priority_queue is a container defined so that the top element is always the one with the highest value.
 *  std::priority_queue is implemented as a container adaptor. Containers adapters are classes that use an
 *  encapsulated container and provide a restricted interface to that container.
 *  The underlying container can be a STL container or some other  container type. It has to provide the
 *  following public member functions: back(), push_back(), pop_back().
 *  priority_queue is defined as a template taking three parameters:
 *  		template < 	class T,
 *  					class Container = vector<T>,
 *  					class Compare = less<typename Container::value_type>
 *  				>
 *  				class priority_queue;
 * 		The first parameter is the type of elements it will contain.
 * 		The second parameter is the underlying container, and is by default std::vector.
 * 		"Compare" is the comparison class. std::less is the default.
 * 		We can provide another comparison class. The class has to be a functor, with it's operator() taking two
 * 		arguments, of the same type as the container elements, and returning a bool. (See  example bellow ).
 * std::priority_queue is defined in the <queue> header.
 */
void TestSTL::test_prority_queue()
{
	using std::priority_queue;

	/**
	*  default constructor
	*  Constructed with a std::vector as the underlying container and std::less as the comparison class.
	*/
	priority_queue<int> pq;

	/**
	* empty function: returns true if the priority_queue contains no elements
	*/
	TESTIFY_ASSERT(pq.empty() == true);

	/**
	*  constructor
	*  Constructed with a std::vector as the underlying container and std::less as the comparison functor.
	*  All the elements from the array will be copied into the pq1.
	*/
	int array[] = { 4, 2, 6, 0, 8 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	priority_queue<int> pq1(array, array + arraySize);

	/**
	* size function: returns the number of elements in the priority_queue
	*/
	TESTIFY_ASSERT(pq1.size() == arraySize);

	/**
	* default constructor
	* Constructed with a std::deque as the underlying container and FunctorForDummy as the comparison class.
	*/
	priority_queue<DummyClass, std::deque<DummyClass>, FunctorForDummy> pq2;

	/**
	* default constructor
	* Constructed with a std::vector as the underlying container and FunctorForDummy as the comparison class.
	*/
	priority_queue<DummyClass, std::vector<DummyClass>, FunctorForDummy> pq3;

	/**
	 * push: Inserts a new element into the priority_queue. The new element is sorted into the queue according
	 * to a comparison functor.
	 * push calls the member function push_back of the underlying container object ( std::vector in our case).
	 */
	DummyClass dA("A");	//lowest value
	DummyClass dB("B");
	DummyClass dC("C");
	DummyClass dD("D");
	DummyClass dE("E");	//highest value
	pq3.push(dB);
	pq3.push(dE);
	pq3.push(dA);
	pq3.push(dC);
	pq3.push(dD);

	/**
	* top function: Returns a constant reference to the element that has the highest value.
	*/
	TESTIFY_ASSERT(pq3.top() == dE);

	/**
	* Removes the element on top of the priority_queue, reducing its size by one.
	*/
	pq3.pop();
	TESTIFY_ASSERT(pq3.top() == dD);
	pq3.pop();
	TESTIFY_ASSERT(pq3.top() == dC);
	pq3.pop();
	TESTIFY_ASSERT(pq3.top() == dB);
	pq3.pop();
	TESTIFY_ASSERT(pq3.top() == dA);
	pq3.pop();
	TESTIFY_ASSERT(pq3.empty() == true);
}
