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
 * @file TestSet.cpp
 * @author Gabriela Rata
 */

#include <set>
#include <utility>  		//for std::pair
#include "../TestSTL.h"
#include "../DummyClass.h"
#include "../FunctorForDummyClass.h"
#include "../LOG.h"

/**
 * test_set: function for testing the set from STL
 * std::set is an associative container, that stores unique elements. Associative container means that,
 * the index doesn't have to be an integer. It can be any type. std::set has, as its first template parameter,
 * the index type, like this:
 *				template <	class Key,
 *						class Compare = less<Key>,
 *						....
						> class set;
 * std::set sorts automatically it's elements from lower to higher. The second template parameter is
 * the ordering criterion, and  is by default std::less<Key>. Another one can be provided and it has to be
 * a class that takes two arguments of the same type as the container elements and returns a bool. (See  example bellow).
 * std::set accepts only one copy of an object. So it's guaranteed that a set will hold unique values. If you try to insert
 * an object that is equivalent to one that is already in the set, the set won't make the insertion.
 * std::set has bidirectional iterators. We can advance with ++ and -- operators, but we can't move more than one step.
 * That means that we can write for example:
 * 				++mySetIterator; 	//ok. Move one step forward
 * 				--mySetIterator; 	//ok. Move one step backward
 * 				mySetIterator + 5; 	//not ok. It won't compile. Only random access iterators can move more then a step...
 * 				mySetIterator - 2	//not ok. It won't compile.
 * std::set is designed to be efficient accessing its elements.
 * std::set is defined in the <set> header
 */
void TestSTL::test_set()
{
	using std::set;

	/**
	 * default constructor
	 * We use the default comparison class, std::less, so it will sort the elements from the lower to the higher.
	 */
	set<int> s1;

	/**
	* empty function: returns true if the set contains no elements
	*/
	TESTIFY_ASSERT(s1.empty() == true);

	/**
	* size function: returns the number of elements
	*/
	TESTIFY_ASSERT(s1.size() == 0);

	/**
	* begin: returns an iterator referring to the first element.
	* If the container is empty, it will return the one past-the-end element in the container,
	* like the set::end() function. See bellow.
	*/
	std::set<int>::iterator itBegin = s1.begin();

	/**
	* end: returns an iterator referring to the one past-the-end element in the container.
	* We must not deference the iterator returned by set::end(). It is used only to see is if we reached the
	* end of the container, when we iterate through it.
	*/
	std::set<int>::iterator itEnd = s1.end();
	//the s1 set is empty, so s1.begin() will return the same value as set::end()
	if( s1.empty() )
	{
		TESTIFY_ASSERT(itBegin == itEnd);
	}
	else
	{
		TESTIFY_ASSERT(itBegin != itEnd);
	}

	/**
	 * insert function: inserts an element in set, at the right place, so that all the elements are in a sorted order.
	 */
	s1.insert(20);
	s1.insert(3);					//it will place 3 before 20.
	s1.insert(55); 					//it will place 55 at the end, as it has the biggest value
	s1.insert(4);   				//it will place 4 after 3
	itBegin = s1.begin();			//refresh the itBegin iterator, as we modified the s1 set.
	TESTIFY_ASSERT(*itBegin == 3);
	++itBegin;						//move to the second element
	TESTIFY_ASSERT(*itBegin == 4);
	++itBegin;						//move forward
	TESTIFY_ASSERT(*itBegin == 20);
	++itBegin;
	TESTIFY_ASSERT(*itBegin == 55);

	/**
	 * insert function: inserts a range of elements in set
	 * We can use another set to add elements to our set, or we can use any container (or array), that
	 * provides a way to iterate through it.
	 */
	int array[] = { 20, 10, 15 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	s1.insert(array, array + arraySize);
	set<int> s1_1;
	s1_1.insert(s1.begin(), s1.end());

	/**
	 * default constructor
	 * The second template parameter, "FunctorForDummy", is the comparison class. The std::set object will use
	 * an object of type "FunctorForDummy" to compare objects of type "DummyClass" and to sort them accordingly.
	 * The sorting is made as we insert elements, so in every moment the elements in a std::set are sorted.
	 */
	set<DummyClass, FunctorForDummy> s2;
	DummyClass smallDummy("A");
	DummyClass bigDummy("C");
	DummyClass middleDummy("B");
	s2.insert(bigDummy);
	s2.insert(smallDummy);		//calls FunctorForDummy::operator()  and inserts the "smallDummy" before the "bigDummy"
	s2.insert(middleDummy);		//calls FunctorForDummy::operator()  and inserts the "middleDummy" in the right place.

	set<DummyClass>::iterator iter = s2.begin();
	TESTIFY_ASSERT(*iter == smallDummy);		//the first element should be the smallest
	++iter; 									//move forward
	TESTIFY_ASSERT(*iter == middleDummy);
	++iter;										//move forward
	TESTIFY_ASSERT(*iter == bigDummy);

	/**
	 * constructor
	 * Iterates between the first and the second iterator (that are provided as parameters),
	 * and makes a copy of each of the elements from that range, inserting it into the container.
	 * In the example bellow, a the new set is constructed and is filled with the all elements from the s3 set.
	 */
	set<DummyClass, FunctorForDummy> s3(s2.begin(), s2.end());
	TESTIFY_ASSERT( s3 == s2 ); 	//set provides besides ==, also !=, <, >, <=, >=.

	/**
	 * copy constructor
	 */
	set<DummyClass, FunctorForDummy> s4(s3);
	TESTIFY_ASSERT( s4 == s3 );

	/**
	 *  erase function: removes a single element, or a range of elements
	 */
	set<DummyClass, FunctorForDummy>::iterator it = s4.begin();
	TESTIFY_ASSERT(*it == smallDummy);		//the first element is smallDummy.
	s4.erase(smallDummy);							//erase the first element from s4.
	it = s4.begin();						//"it" iterator was invalidated by the erase. Have to retrieve again the first element
	TESTIFY_ASSERT( *it == middleDummy );
	s4.erase(s4.begin(), s4.end());			//erase all the elements
	TESTIFY_ASSERT(s4.empty());

	/**
	 * clear function: all the elements are destroyed and removed from the set.
	 * After calling this function the set will have the size zero.
	 */
	s3.clear();
	TESTIFY_ASSERT(s3.size() == 0);

	/**
	 * find function: Searches the container for an element and returns an iterator
	 * to it if found. If not it returns an iterator to the element past the end of the container (set::end).
	 * set::find() member function is much faster than the generic std::find( ) algorithm (from <algorithm>).
	 */
	set<DummyClass, FunctorForDummy>::iterator found = s3.find(smallDummy); //s3 empty, nothing to find
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
	int a[] = { 3, 5, 1, 7 };
	int aSize = sizeof(a)/sizeof(a[0]);
	set<int> s5(a, a + aSize); 							//set will contain: 1, 3, 5, 7
	set<int>::iterator lowerBound = s5.lower_bound(3);  //returns 3 (the first element >= 3)
	TESTIFY_ASSERT( 3 == *lowerBound);					//3>=3

	/**
	 * upper_bound: Returns an iterator pointing to the first element in the container which compares
	 * strictly greater than the value provided as argument (using the container's comparison object).
	 */
	set<int>::iterator upperBound = s5.upper_bound(3);
	TESTIFY_ASSERT( 5 == *upperBound );				   // 5>3

	/**
	 * equal_range:
	 * prototype: pair<iterator, iterator> equal_range ( const key_type &someKey ) const;
	 * Returns the bounds of a range that includes all the elements in the container with a
	 * key that compares equal to "someKey".
	 * In set no duplicate keys are allowed, so the range will include only one element.
	 * If "someKey" does not match any key in the container, the range returned has a length of zero,
	 * with both iterators pointing to the nearest value greater than "someKey",
	 * If "someKey" is the greater than all the elements  set::end is returned.
	 */
	std::pair< set<int>::iterator, set<int>::iterator > equal = s5.equal_range(3);
	TESTIFY_ASSERT( 3 == *equal.first );
	TESTIFY_ASSERT( 5 == *equal.second );
}
