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

#include <assert.h>
#include <set>
#include <utility>  		//for std::pair
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"


void STLMoblet::STL_set()
{
	LOG("\n");
	LOG("========================= set  ==================================================================================");

	LOG("/**");
	LOG(" * test_set: function for testing the set from STL");
	LOG(" * std::set is an associative container, that stores unique elements. Associative container means that,");
	LOG(" * the index doesn't have to be an integer. It can be any type. std::set has, as its first template parameter,");
	LOG(" * the index type, like this:");
	LOG(" * 			template <	class Key,");
	LOG(" * 				class Compare = less<Key>,");
	LOG(" * 				....");
	LOG(" *					> class set;");
	LOG(" * std::set sorts automatically it's elements from lower to higher. The second template parameter is");
	LOG(" * the ordering criterion, and  is by default std::less<Key>. Another one can be provided and it has to be");
	LOG(" * a class that takes two arguments of the same type as the container elements and returns a bool. (See  example bellow).");
	LOG(" * std::set accepts only one copy of an object. So it's guaranteed that a set will hold unique values. If you try to insert");
	LOG(" * an object that is equivalent to one that is already in the set, the set won't make the insertion.");
	LOG(" * std::set has bidirectional iterators. We can advance with ++ and -- operators, but we can't move more than one step.");
	LOG(" * That means that we can write for example:");
	LOG(" * 				++mySetIterator; 	//ok. Move one step forward");
	LOG(" * 				--mySetIterator; 	//ok. Move one step backward");
	LOG(" * 				mySetIterator + 5; 	//not ok. It won't compile. Only random access iterators can move more then a step.");
	LOG(" * 				mySetIterator - 2	//not ok. It won't compile.");
	LOG(" * std::set is designed to be efficient accessing its elements.");
	LOG(" * std::set is defined in the <set> header");
	LOG(" */");
	LOG("\n");

	using std::set;

	LOG("/**");
	LOG(" * default constructor");
	LOG(" * We use the default comparison class, std::less, so it will sort the elements from the lower to the higher.");
	LOG(" */");

	TRACE(set<int> s1;);

	LOG("/**");
	LOG("* empty function: returns true if the set contains no elements");
	LOG("*/");
	bool e = s1.empty();
	log_to_console(e, "bool e = s1.empty();	// e = ");

	LOG("\n");
	LOG("/**");
	LOG("* size function: returns the number of elements");
	LOG("*/");
	int sz = s1.size();
	log_to_console(sz, "int sz = s1.size(); 	// sz = ");

	LOG("\n");
	LOG("/**");
	LOG("* begin: returns an iterator referring to the first element.");
	LOG("* If the container is empty, it will return the one past-the-end element in the container,");
	LOG("* like the set::end() function. See bellow.");
	LOG("*/");

	TRACE(std::set<int>::iterator itBegin = s1.begin(););

	LOG("\n");
	LOG("/**");
	LOG("* end: returns an iterator referring to the one past-the-end element in the container.");
	LOG("* We must not deference the iterator returned by set::end(). It is used only to see is if we reached the");
	LOG("* end of the container, when we iterate through it.");
	LOG("*/");

	TRACE(std::set<int>::iterator itEnd = s1.end(););
	LOG("//the s1 set is empty, so s1.begin() will return the same value as set::end()");
	TRACE(assert(itBegin == itEnd););

	LOG("\n");
	LOG("/**");
	LOG(" * insert function: inserts an element in set, at the right place, so that all the elements are in a sorted order.");
	LOG(" */");

	TRACE(s1.insert(20););
	s1.insert(3);
	log_to_console(s1, "s1.insert(3);	//it will place 3 before 20 => s1 will contain: ");

	s1.insert(55);
	log_to_console(s1, "s1.insert(55); 	//it will place 55 at the end, as it has the biggest value => s1 will contain: ");

	s1.insert(4);
	log_to_console(s1, "s1.insert(4);	//it will place 4 after 3 => s1 will contain: ");

	LOG("\n");
	log_to_console(s1, "//s1 contains now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * insert function: inserts a range of elements in set");
	LOG(" * We can use another set to add elements to our set, or we can use any container (or array), that");
	LOG(" * provides a way to iterate through it.");
	LOG(" */");

	int array[] = { 20, 10, 15 };
	LOG("int array[] = { 20, 10, 15 };");
	TRACE(int arraySize = sizeof(array)/sizeof(array[0]););
	s1.insert(array, array + arraySize);
	log_to_console(s1, "s1.insert(array, array + arraySize);	//s1 will contain: ");

	LOG("\n");
	TRACE(set<int> s1_1;);
	s1_1.insert(s1.begin(), s1.end());
	log_to_console(s1_1, "s1_1.insert(s1.begin(), s1.end());	//s1_1 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * default constructor");
	LOG(" * The second template parameter, \"LessExperienceFunctor\", is the comparison class. The std::set object will use");
	LOG(" * an object of type \"LessExperienceFunctor\" to compare objects of type \"Employee\" and to sort them accordingly.");
	LOG(" * The sorting is made as we insert elements, so in every moment the elements in a std::set are sorted.");
	LOG(" */");
	LOG("\n");

	set<Employee, LessExperienceFunctor> s2;
	LOG("set<Employee, LessExperienceFunctor> s2;");

	TRACE(Employee junior(1, "Jim"););
	TRACE(Employee middle(3, "Sam"););
	TRACE(Employee senior(5, "Bob"););

	TRACE(s2.insert(senior););
	TRACE(s2.insert(middle););	//calls LessSalaryFunctor::operator()  and inserts the "middle" before the "senior"
	TRACE(s2.insert(junior););	//calls LessSalaryFunctor::operator()  and inserts the "junior" before the "middle"

	log_to_console(s2, "//s2 contains now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * Iterates between the first and the second iterator (that are provided as parameters),");
	LOG(" * and makes a copy of each of the elements from that range, inserting it into the container.");
	LOG(" * In the example bellow, a the new set is constructed and is filled with the all elements from the s3 set.");
	LOG(" */");

	set<Employee, LessExperienceFunctor> s3(s2.begin(), s2.end());
	log_to_console(s3, "set<Employee, LessSalaryFunctor> s3(s2.begin(), s2.end());	//s3 will contain: ");

	LOG("\n");
	LOG("//set overloads the following operators: ==, !=, <, >, <=, >=.");
	TRACE(assert( s3 == s2 ););

	LOG("\n");
	LOG("/**");
	LOG(" * copy constructor");
	LOG(" */");

	set<Employee, LessExperienceFunctor> s4(s3);
	log_to_console(s4, "set<Employee, LessExperienceFunctor> s4(s3);	//s4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  erase function: removes a single element, or a range of elements");
	LOG(" */");

	set<Employee, LessExperienceFunctor>::iterator it = s4.begin();
	log_to_console(*it, "//set<Employee, LessExperienceFunctor>::iterator it = s4.begin(); 	//*it = ");

	TRACE(s4.erase(junior););
	it = s4.begin();
	log_to_console(*it, "it = s4.begin(); 	//*it = ");

	LOG("\n");
	TRACE(s4.erase(middle););
	it = s4.begin();
	log_to_console(*it, "it = s4.begin(); 	//*it = ");

	LOG("\n");
	LOG("/**");
	LOG(" * clear function: all the elements are destroyed and removed from the set.");
	LOG(" * After calling this function the set will have the size zero.");
	LOG(" */");

	TRACE(s3.clear(););
	int s3Size = s3.size();
	log_to_console(s3Size, "int s3Size = s3.size();	//s3Size = ");

	LOG("\n");
	LOG("/**");
	LOG(" * find function: Searches the container for an element and returns an iterator");
	LOG(" * to it if found. If not it returns an iterator to the element past the end of the container (set::end).");
	LOG(" * set::find() member function is much faster than the generic std::find( ) algorithm (from <algorithm>).");
	LOG(" */");

	set<Employee,LessExperienceFunctor>::iterator found = s3.find(junior);
	TRACE(assert(found == s3.end()));

	Employee someE(4, "Phil");
	s3.insert(someE);
	found = s3.find(someE);
	log_to_console(*found, "found = s3.find(someE);	//found = ");

	LOG("\n");
	LOG("/**");
	LOG("* lower_bound: Returns an iterator referring to the first element in the container which is not");
	LOG(" * less (using the container's comparison object) than the value provided as parameter.");
	LOG(" */");
	LOG("\n");

	int a[] = { 3, 5, 1, 7 };
	LOG("int a[] = { 3, 5, 1, 7 };");
	TRACE(int aSize = sizeof(a)/sizeof(a[0]););

	set<int> s5(a, a + aSize); 							//set will contain: 1, 3, 5, 7
	log_to_console(s5, "set<int> s5(a, a + aSize); 	//s5 contains: ");

	set<int>::iterator lowerBound = s5.lower_bound(3);  //returns 3 (the first element >= 3)
	log_to_console(*lowerBound, "set<int>::iterator lowerBound = s5.lower_bound(3);	//*lowerBound = ");

	LOG("\n");
	LOG("/**");
	LOG(" * upper_bound: Returns an iterator pointing to the first element in the container which compares");
	LOG(" * strictly greater than the value provided as argument (using the container's comparison object).");
	LOG(" */");

	set<int>::iterator upperBound = s5.upper_bound(3);
	log_to_console(*upperBound, "set<int>::iterator upperBound = s5.upper_bound(3);	//*upperBound = ");

	LOG("\n");
	LOG("/**");
	LOG(" * equal_range:");
	LOG(" * prototype: pair<iterator, iterator> equal_range ( const key_type &someKey ) const;");
	LOG(" * Returns the bounds of a range that includes all the elements in the container with a");
	LOG(" * key that compares equal to \"someKey\".");
	LOG(" * In set no duplicate keys are allowed, so the range will include only one element.");
	LOG(" * If \"someKey\" does not match any key in the container, the range returned has a length of zero,");
	LOG(" * with both iterators pointing to the nearest value greater than\"someKey\",");
	LOG(" * If \"someKey\" is the greater than all the elements  set::end is returned.");
	LOG(" */");

	std::pair< set<int>::iterator, set<int>::iterator > equal = s5.equal_range(3);
	LOG("std::pair< set<int>::iterator, set<int>::iterator > equal = s5.equal_range(3);");
	log_to_console(*equal.first, "//*equal.first = ");
	log_to_console(*equal.second, "//*equal.second = ");

	LOG("\n");
}
