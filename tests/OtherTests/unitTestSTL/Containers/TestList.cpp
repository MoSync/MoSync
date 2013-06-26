/*
 * TestList.cpp
 *
 *  Created on: May 30, 2011
 *      Author: gabi
 */

#include <list>
#include "../TestSTL.h"

bool SomeUnaryPredToTestRemove(int val)
{
	return (val==10);
}

/**
 *  test function for STL container std::list
 *  std::list is implemented as a doubly linked list of elements
 *  std::list doesn't have random access, it provides only bidirectional iterators. For accessing a element, you have to
 *  iterate through list, until you reach the element. Compared with std::vector or std::deque is not efficient,
 *  when accessing an random element.
 *  We can write for example:
 * 				++myListIterator; 	//ok. Move one step forward
 * 				--myListIterator; 	//ok. Move one step backward
 * 				myListIterator + 5; //not ok. It won't compile. Only random access iterators can move more then a step...
 * 				myListIterator - 2	//not ok. It won't compile.
 *  std::list is efficient for insertion and removal of elements anywhere in the container. (From this point of view is more
 *  efficient than vector or deque.)
 *	std::list is defined in <list> header
 */

void TestSTL::test_list()
{

	using std::list;
	/**
	 * Default constructor
	 */
	std::list<int> myList;

	/**
	* empty function: returns true if the list contains no elements
    */
	TESTIFY_ASSERT(myList.empty() == true);

	/**
	 * constructor
	 * fills the list with 20 elements (of value -1).
	 */
	list<int> lst1(20, -1);
	TESTIFY_ASSERT(lst1.size() == 20);

	/**
	 * constructor
	 * constructs a list from an array
	 */
	int array[]= {1, 2, 3, 4, 5};
	int numElements = sizeof(array)/sizeof(array[0]);
	list<int> lst2(array, array + numElements);
	TESTIFY_ASSERT(lst2.size() == numElements);

	/**
	 * copy constructor
	 */
	list<int> lst2_copy(lst2);
	TESTIFY_ASSERT( lst2.size() == lst2_copy.size());

	/**
	 * assignment operator
	 */
	list<int> lst3;
	lst3 = lst2;
	TESTIFY_ASSERT(lst3.size() == lst2.size());

	/**
	 * assign function
	 * copies all the elements of lst3 to lst4.
	 */
	list<int> lst4;
	lst4.assign(lst3.begin(), lst3.end());
	TESTIFY_ASSERT( lst4 == lst3 );

	/**
	 * clear function: all the elements in the list are destroyed and removed from the list,
	 * after calling this function the list will have the size zero.
	 */
	lst4.clear();
	TESTIFY_ASSERT(lst4.size() == 0);

	/**
	 * resize function: resizes the list to contain the specified number of elements
	 * prototype: void resize(size_t newSize, T c = T())
	 * If the specified size is smaller than the current size, the content is reduced
	 * to the first "newSize" elements, and the rest are destroyed.
	 */
	lst4.resize(4);
	TESTIFY_ASSERT(lst4.size() == 4);

	/**
	 * overload version of the assign function
	 * fills the vector with 10 elements, having the value "-6"
	 */

	lst4.assign(10, -6);
	TESTIFY_ASSERT( lst4.size() == 10);

	/**
	 * push_back: adds an element at the end of the vector
	 */
	lst4.push_back(99);
	TESTIFY_ASSERT( lst4.size() == 11 );

	/**
	 * push_front: adds an element at the beginning of the vector
	 */
	lst4.push_front(99);
	TESTIFY_ASSERT( lst4.size() == 12 );

	/**
	 * pop_back: removes and destroys the last element of the list (by resizing it by one)
	 */
	lst4.pop_back();
	TESTIFY_ASSERT( lst4.size() == 11 );

	/**
	 * pop_front: removes and destroys the first element of the list (by resizing it by one)
	 */
	lst4.pop_front();
	TESTIFY_ASSERT( lst4.size() == 10 );

	/**
	 * begin: returns an iterator referring to the first element of the vector
	 */
	std::list<int>::iterator itBegin = lst4.begin();

	/**
	 * end: returns an iterator referring to the one past the end element in the list
	 * We must not deference the iterator returned by end(). It is used to see is if we reached the
	 * end of the container, when we iterate through it.
	 */
	std::list<int>::iterator itEnd = lst4.end();
	if(!lst4.empty())
	{
		TESTIFY_ASSERT( itBegin != itEnd );
	}
	//iterate through lst4
	std::list<int>::iterator iter;
	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//adds 11 to each element of deque
		*iter += 11;
	}

	/**
	 * insert function: inserts an element before the specified position.
	 * The position before it should insert the element is indicated by an iterator
	 * inserts the value "1001", before the first element. The first element will be 1001,
	 * after this function call.
	 */
	lst4.insert(itBegin, 1001);

	/**
	 * inserts 7 elements, with the value -90, before the first element of v4.
	 */
	itBegin = lst4.begin();
	lst4.insert(itBegin, 30, -90);

	/**
	 *  remove function: removes all the elements with the specified value ( -90 ).
	 */
	itBegin = lst4.begin();
	lst4.erase(itBegin);
	//erase the next two elements. The list is not an random
	//access iterator, so we have to move the iterator step by step
	//e.g: itBegin + 2 illegal
	itBegin = lst4.begin();
	list<int>::iterator it = itBegin;
	++it;
	++it;
	lst4.erase(itBegin, it);

	/**
	 *  remove function: removes all the elements with the specified value ( -90 ).
	 */
	lst4.remove(-90);
	lst4.remove_if(&SomeUnaryPredToTestRemove);


	/**
	 *  merge function: for l1.merge(l2) merges l2 into the l1, inserting all the elements of l2 into
	 *  the l1 object at their respective ordered positions. This empties l2 and increases the l1 size.
	 */
	int a1[] = { 0, 2, 4, 6, 8, 10 };
	int a2[] = { 1, 3, 5, 7, 9 };
	list<int> even(a1, a1 + sizeof(a1)/sizeof(a1[0]));
	list<int> uneven(a2, a2 + sizeof(a2)/sizeof(a2[0]));
	even.merge(uneven);
	int i = 0;
	for(std::list<int>::iterator it = even.begin(); it!= even.end(); ++it, ++i)
	{
		TESTIFY_ASSERT(i == *it);
	}
	TESTIFY_ASSERT(uneven.size() == 0);

	/**
	 *  reverse function: reverses the order of the elements.
	 */
	even.reverse();
	i = 10;
	for(std::list<int>::iterator it = even.begin(); it!= even.end(); ++it, --i)
	{
		TESTIFY_ASSERT(i == *it);
	}


	/**
	 *  sort function: sorts the elements in the container from lower to higher.
	 *  By default the comparison between elements is made with the < operator,
	 *  but an predicate can be provided also, if a custom comparison is required.
	 *  Sorting the list is done not by and moving (and copying) of objects, but
	 *  by changing the links. Therefore is more efficient to use the
	 *  sort member function, than the template function std::sort.
	 */
	even.sort();
	i = 0;
	for(std::list<int>::iterator it = even.begin(); it!= even.end(); ++it, ++i)
	{
		TESTIFY_ASSERT(i == *it);
	}

	/**
	 *  splice function: Moves elements from one list into the another at the specified
	 *  position. It inserts the specified elements into the destination container
	 *  and removes them from the source container.
	 */
	int values1[] = {6, 3, 5};
	int numElem1 = sizeof(values1)/sizeof(values1[0]);
	int values2[] = {2, 89 };
	int numElem2 = sizeof(values2)/sizeof(values2[0]);
	list<int> source(values1, values1 + numElem1 );
	list<int> destination(values2, values2 + numElem2);
	list<int>::iterator position = destination.begin();
	destination.splice(position, source);
	TESTIFY_ASSERT(destination.size() == (numElem1 + numElem2));  //destination contains now:O 6, 3, 5, 2, 89.
	TESTIFY_ASSERT(source.size() == 0);

}
