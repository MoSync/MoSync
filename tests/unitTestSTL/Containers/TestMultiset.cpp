/*
 * TestMultiset.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */

#include <set>
#include <utility>  		//for std::pair
#include "../TestSTL.h"
#include "../DummyClass.h"
#include "../LOG.h"

/**
 * We have to provide a way to compare two DummyClass objects.
 * We didn't provide an operator< in DummyClass, so we have to provide some comparison criterion that enables multiset
 * to sort its elements.
 * If we had provided an operator < for the DummyClass, then we could use the default comparison class (std::less).
 * That would have called our operator< automatically.
 */

struct PredicateForMultiset
{
	bool operator()(const DummyClass &a, const DummyClass &b) const
	{
		return a.getName() < b.getName();
	}
};

/**
 * test_multiset: function for testing the multiset
 * std::multiset is an associative container. Associative container means that,
 * the index doesn't have to be an integer. It can be any type. std::multiset has, as its first template parameter,
 * the index type, like this:
 * 			template <	class Key,
 * 						class Compare = less<Key>,
 * 						....
           	    	  > class multiset;
 * std::multiset sorts automatically it's elements from lower to higher. The second template parameter is
 * the ordering criterion, and  is by default std::less<Key>. Another orsering criterion can be provided, and it has to be
 * a class that takes two arguments of the same type as the container elements, and returns a bool. (See  example bellow).
 * std::multiset accepts multiple copies of an object.
 * std::multiset has bidirectional iterators. We can advance with ++ and -- operators, but we can't move more than one step.
 * That means that we can write for example:
 * 				++myMultisetIterator; 	//ok. Move one step forward
 * 				--myMultisetIterator; 	//ok. Move one step backward
 * 				myMultisetIterator + 5; //not ok. It won't compile. Only random access iterators can move more then a step...
 * 				myMultisetIterator - 2	//not ok. It won't compile.
 * std::multiset is designed to be efficient accessing its elements.
 * std::multiset is defined in the <set> header
 */
void TestSTL::test_multiset()
{
	using std::multiset;

	/**
	 * default constructor
	 * We use the default comparison class, std::less, so it will sort the elements from the lower to the higher.
	 */
	multiset<int> s1;

	/**
	* empty function: returns true if the multiset contains no elements
	*/
	TESTIFY_ASSERT(s1.empty() == true);

	/**
	* size function: returns the number of elements
	*/
	TESTIFY_ASSERT(s1.size() == 0);

	/**
	* begin: returns an iterator referring to the first element.
	* If the container is empty, it will return the one past-the-end element in the container,
	* like the multiset::end() function. See bellow.
	*/
	std::multiset<int>::iterator itBegin = s1.begin();

	/**
	* end: returns an iterator referring to the one past-the-end element in the container.
	* We must not deference the iterator returned by multiset::end(). It is used only to see is if we reached the
	* end of the container, when we iterate through it.
	*/
	std::multiset<int>::iterator itEnd = s1.end();
	//the s1 multiset is empty, so s1.begin() will return the same value as multiset::end()
	if( s1.empty() )
	{
		TESTIFY_ASSERT(itBegin == itEnd);
	}
	else
	{
		TESTIFY_ASSERT(itBegin != itEnd);
	}

	/**
	 * insert function: inserts an element in multiset, at the right place, so that all the elements are in a sorted order.
	 */
	s1.insert(20);
	s1.insert(3);					//it will place 3 before 20.
	s1.insert(55); 					//it will place 55 at the end, as it has the biggest value
	s1.insert(4);   				//it will place 4 after 3
	s1.insert(3);
	s1.insert(3);
	//s1 contains now the values: 3, 3, 3, 4, 20, 55
	itBegin = s1.begin();			//refresh the itBegin iterator, as we modified the s1 multiset.
	TESTIFY_ASSERT(*itBegin == 3);
	++itBegin;						//move to the second element
	TESTIFY_ASSERT(*itBegin == 3);
	++itBegin;
	TESTIFY_ASSERT(*itBegin == 3);
	++itBegin;
	TESTIFY_ASSERT(*itBegin == 4);
	++itBegin;						//move forward
	TESTIFY_ASSERT(*itBegin == 20);
	++itBegin;
	TESTIFY_ASSERT(*itBegin == 55);

	/**
	 * insert function: inserts a range of elements in multiset
	 * We can use another multiset to add elements to our multiset, or we can use any container (or array), that
	 * provides a way to iterate through it.
	 */
	int array[] = { 20, 10, 15, 10 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	s1.insert(array, array + arraySize);
	multiset<int> s1_1;
	s1_1.insert(s1.begin(), s1.end());

	/**
	 * default constructor
	 * The second template parameter, "PredicateForMultiset", is the comparison class. The std::multiset object will use
	 * an object of type "PredicateForMultiset" to compare objects of type "DummyClass" and to sort them accordingly.
	 * The sorting is made as we insert elements, so in every moment the elements in a std::multiset are sorted.
	 */
	multiset<DummyClass, PredicateForMultiset> s2;
	DummyClass smallDummy("A");
	DummyClass bigDummy("C");
	DummyClass middleDummy("B");
	s2.insert(bigDummy);
	s2.insert(smallDummy);		//calls PredicateForMultiset::operator()  and inserts the "smallDummy" before the "bigDummy"
	s2.insert(middleDummy);		//calls PredicateForMultiset::operator()  and inserts the "middleDummy" in the right place.
	s2.insert(smallDummy);		//inserts again a smallDumy object (before "middleDummy").

	multiset<DummyClass>::iterator iter = s2.begin();
	TESTIFY_ASSERT(*iter == smallDummy);		//the first element should be the smallest
	++iter;
	TESTIFY_ASSERT(*iter == smallDummy);
	++iter;
	TESTIFY_ASSERT(*iter == middleDummy);
	++iter;
	TESTIFY_ASSERT(*iter == bigDummy);

	/**
	 * constructor
	 * Iterates between the first and the second iterator (that are provided as parameters),
	 * and makes a copy of each of the elements from that range, inserting it into the container.
	 * In the example bellow, a the new multiset is constructed and is filled with the all elements from the s3 set.
	 */
	multiset<DummyClass, PredicateForMultiset> s3(s2.begin(), s2.end());
	TESTIFY_ASSERT( s3 == s2 ); 	//multiset provides besides ==, also !=, <, >, <=, >=.

	/**
	 * copy constructor
	 */
	multiset<DummyClass, PredicateForMultiset> s4(s3);
	TESTIFY_ASSERT( s4 == s3 );

	/**
	 *  erase function: removes elements. It will remove all the elements with the specified value.
	 */
	multiset<DummyClass, PredicateForMultiset>::iterator it = s4.begin();
	TESTIFY_ASSERT(*it == smallDummy);		//Erases all the elements equal to "smallDummy"
	s4.erase(smallDummy);
	it = s4.begin();						//"it" iterator was invalidated by the erase. Have to retrieve again the first element
	TESTIFY_ASSERT(*it == middleDummy );
	s4.erase(s4.begin(), s4.end());			//erase all the elements
	TESTIFY_ASSERT(s4.empty());

	/**
	 * clear function: all the elements are destroyed and removed from the set.
	 * After calling this function the multiset will have the size zero.
	 */
	s3.clear();
	TESTIFY_ASSERT(s3.size() == 0);

	/**
	 * find function: Searches the container for an element and returns an iterator to it if found.
	 * The function returns an iterator to one of the elements with that value, even if there are more than one
	 * elements having that value. To get all the elements use multiset::equal_range.
	 * If it doesn't find an element it returns an iterator to the element past the end of the container (set::end).
	 * multiset::find() member function is much faster than the generic std::find( ) algorithm (from <algorithm>).
	 */
	multiset<DummyClass, PredicateForMultiset>::iterator found = s3.find(smallDummy); //s3 empty, nothing to find
	TESTIFY_ASSERT( s3.end() == found );
	DummyClass f1("found me");
	s3.insert(f1);
	found = s3.find(f1);
	TESTIFY_ASSERT( *found == f1);
	DummyClass f2("I not there");
	found = s3.find(f2);
	TESTIFY_ASSERT( s3.end() == found);

	/**
	 * lower_bound: Returns an iterator referring to the first element in the container which is not
	 * less (using the container's comparison object) than the value provided as parameter.
	 */
	int a[] = { 3, 5, 1, 7, 3, 3 };
	int aSize = sizeof(a)/sizeof(a[0]);
	multiset<int> s5(a, a + aSize); 							//set will contain: 1, 3, 5, 7
	multiset<int>::iterator lowerBound = s5.lower_bound(3);  //returns 3 (the first element >= 3)
	TESTIFY_ASSERT( 3 == *lowerBound);					//3>=3

	/**
	 * upper_bound: Returns an iterator pointing to the first element in the container which compares
	 * strictly greater than the value provided as argument (using the container's comparison object).
	 */
	multiset<int>::iterator upperBound = s5.upper_bound(3);
	TESTIFY_ASSERT( 5 == *upperBound );				   // 5>3

	/**
	 * equal_range:
	 * prototype: pair<iterator, iterator> equal_range ( const key_type &someKey ) const;
	 * Returns the bounds of a range that includes all the elements in the container with a
	 * key that compares equal to "someKey".
	 * If "someKey" does not match any key in the container, the range returned has a length of zero,
	 * with both iterators pointing to the nearest value greater than "someKey",
	 * If "someKey" is the greater than all the elements  set::end is returned.
	 */
	std::pair< multiset<int>::iterator, multiset<int>::iterator > equal = s5.equal_range(3);
	TESTIFY_ASSERT( 3 == *equal.first );
	TESTIFY_ASSERT( 5 == *equal.second );
	for(multiset<int>::iterator it = equal.first; it!= equal.second; ++it)
	{
		TESTIFY_ASSERT( 3 == *it );
	}
}
