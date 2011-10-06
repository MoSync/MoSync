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
 * @file Example_using_list.cpp
 * @author Gabriela Rata
 */

#include <list>
#include "../STLMoblet.h"
#include "../LOG.h"

bool SomeUnaryPredToTestRemove(int val)
{
	return (val==10);
}

void STLMoblet::STL_list()
{
	LOG("\n");
	LOG("========================= list ==================================================================================");

	LOG("/**");
	LOG(" *  std::list is implemented as a doubly linked list of elements");
	LOG(" *  std::list doesn't have random access, it provides only bidirectional iterators. For accessing a element, you have to");
	LOG(" *  iterate through list, until you reach the element. Compared with std::vector or std::deque is not efficient,");
	LOG(" *  when accessing an random element.");
	LOG(" *  We can write for example:");
	LOG(" * 				++myListIterator; 	//ok. Move one step forward");
	LOG(" * 				--myListIterator; 	//ok. Move one step backward");
	LOG(" * 				myListIterator + 5; 	//not ok. It won't compile. Only random access iterators can move more then a step...");
	LOG(" * 				myListIterator - 2	//not ok. It won't compile.");
	LOG(" *  std::list is efficient for insertion and removal of elements anywhere in the container. (From this point of view is more");
	LOG(" *  efficient than vector or deque.)");
	LOG(" *	std::list is defined in <list> header");
	LOG(" */");
	LOG("\n");

	using std::list;

	LOG("/**");
	LOG("* Default constructor");
	LOG(" */");
	TRACE(std::list<int> myList;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the list contains no elements");
	LOG("*/");
	bool e = myList.empty();
	log_to_console((bool)e, "bool e = myList.empty(); 	// e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * fills the list with 20 elements (of value -1).");
	LOG(" */");

	list<int> lst1(20, -1);
	log_to_console(lst1, "list<int> lst1(20, -1); 	//lst1 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * constructs a list from an array");
	LOG(" */");
	int array[]= {1, 2, 3, 4, 5};
	log_to_console("int array[]= {1, 2, 3, 4, 5};");

	TRACE(int numElements = sizeof(array)/sizeof(array[0]););

	list<int> lst2(array, array + numElements);
	log_to_console(lst2, "list<int> lst2(array, array + "
			"numElements); 	//lst2 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * copy constructor");
	LOG(" */");

	list<int> lst2_copy(lst2);
	log_to_console(lst2_copy, "list<int> lst2_copy(lst2); 	"
			"//lst2_copy will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * assignment operator");
	LOG(" */");

	TRACE(list<int> lst3;);
	lst3 = lst2;
	log_to_console(lst3, "lst3 = lst2; 	//lst3 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * assign function");
	LOG(" * copies all the elements of lst3 to lst4.");
	LOG(" */");

	TRACE(list<int> lst4;);
	lst4.assign(lst3.begin(), lst3.end());
	log_to_console(lst4,
			"lst4.assign(lst3.begin(), lst3.end()); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * clear function: all the elements in the list are destroyed and removed from the list,");
	LOG(" * after calling this function the list will have the size zero.");
	LOG(" */");

	lst4.clear();
	log_to_console((int)lst4.size(), "lst4.clear(); 	//lst.size() returns now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * resize function: resizes the list to contain the specified number of elements");
	LOG(" * prototype: void resize(size_t newSize, T c = T())");
	LOG(" * If the specified size is smaller than the current size, the content is reduced");
	LOG(" * to the first \"newSize\" elements, and the rest are destroyed.");
	LOG(" */");

	lst4.resize(4);
	log_to_console(lst4, "lst4.resize(4) 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * overload version of the assign function");
	LOG(" * fills the vector with 10 elements, having the value \"-6\"");
	LOG(" */");

	lst4.assign(10, -6);
	log_to_console(lst4, "lst4.assign(10, -6); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * push_back: adds an element at the end of the vector");
	LOG(" */");

	lst4.push_back(99);
	log_to_console(lst4, "lst4.push_back(99); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * push_front: adds an element at the beginning of the vector");
	LOG(" */");

	lst4.push_front(99);
	log_to_console(lst4, "lst4.push_front(99); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop_back: removes and destroys the last element of the list (by resizing it by one)");
	LOG(" */");

	lst4.pop_back();
	log_to_console(lst4, "lst4.pop_back(); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop_front: removes and destroys the first element of the list (by resizing it by one)");
	LOG(" */");

	lst4.pop_front();
	log_to_console(lst4, "lst4.pop_front(); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * begin: returns an iterator referring to the first element of the vector");
	LOG(" */");

	TRACE(std::list<int>::iterator itBegin = lst4.begin(););
	log_to_console(*itBegin, "*itBegin = ");

	LOG("\n");
	LOG("/**");
	LOG(" * end: returns an iterator referring to the one past the end element in the list");
	LOG(" * We must not deference the iterator returned by end(). It is used to see is if we reached the");
	LOG(" * end of the container, when we iterate through it.");
	LOG(" */");

	TRACE(std::list<int>::iterator itEnd = lst4.end(););

	std::list<int>::iterator iter;
	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//add 11 to each element of list
		*iter += 11;
	}

	log_to_console("//add 11 to each element of list");
	LOG("for(iter = itBegin; iter != itEnd; ++iter)");
	LOG("{");
	LOG("	*iter += 11;");
	LOG("}");
	log_to_console(lst4, "//lst4 contains now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * insert function: inserts an element before the specified position.");
	LOG(" * The position before it should insert the element is indicated by an iterator");
	LOG(" * inserts the value \"1001\", before the first element. The first element will be 1001,");
	LOG(" * after this function call.");
	LOG(" */");

	lst4.insert(itBegin, 1001);
	log_to_console(lst4, "lst4.insert(itBegin, 1001); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * inserts 3 elements, with the value -90, before the first element of v4.");
	LOG(" */");

	TRACE(itBegin = lst4.begin(););
	lst4.insert(itBegin, 3, -90);
	log_to_console(lst4, "lst4.insert(itBegin, 2, -90); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  erase function: removes an element, or a range of elements from the list");
	LOG(" */");

	TRACE(itBegin = lst4.begin(););

	lst4.erase(itBegin);
	log_to_console(lst4, "lst4.erase(itBegin); 	//lst4 will contain: ");

	LOG("\n");
	LOG("//We want to erase the first three elements from the list."
			"The list doesn't have a random access iterator, so we have to move "
			"the iterator step by step.");
	LOG("//e.g: itBegin + 2 is illegal");
	LOG("\n");

	TRACE(itBegin = lst4.begin(););
	TRACE(list<int>::iterator it = itBegin;);
	TRACE(++it;);
	TRACE(++it;);
	TRACE(++it;);
	lst4.erase(itBegin, it);

	log_to_console(lst4, "lst4.erase(itBegin, it); 	//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  remove function: removes all the elements with the specified value ( -90 ).");
	LOG(" */");

	lst4.remove(-90);
	log_to_console(lst4, "lst4.remove(-90); 	//lst4 will contain: ");

	LOG("\n");
	LOG("//We define a predicate function to be used with list::remove_if function:\n");
	LOG("bool SomeUnaryPredToTestRemove(int val)");
	LOG("{");
	LOG("	return (val==10);");
	LOG("}");

	LOG("\n");
	lst4.remove_if(&SomeUnaryPredToTestRemove);
	log_to_console(lst4, "lst4.remove_if(&SomeUnaryPredToTestRemove); 	"
			"//lst4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  merge function: for l1.merge(l2) merges l2 into the l1, inserting all the elements of l2 into");
	LOG(" *  the l1 object at their respective ordered positions. This empties l2 and increases the l1 size.");
	LOG(" */");
	LOG("\n");

	int a1[] = { 0, 2, 4, 6, 8, 10 };
	list<int> even(a1, a1 + sizeof(a1)/sizeof(a1[0]));

	int a2[] = { 1, 3, 5, 7, 9 };
	list<int> uneven(a2, a2 + sizeof(a2)/sizeof(a2[0]));

	log_to_console(even, "//\"even\" list contains: ");
	log_to_console(uneven, "//\"uneven\" list contains: ");

	even.merge(uneven);
	log_to_console(even, "even.merge(uneven);	//even will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  reverse function: reverses the order of the elements.");
	LOG(" */");
	even.reverse();
	log_to_console(even, "even.reverse();	//even will contain: ");
	int crtSize = uneven.size();
	log_to_console(crtSize, "int crtSize = uneven.size();	//crtSize = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  sort function: sorts the elements in the container from lower to higher.");
	LOG(" *  By default the comparison between elements is made with the < operator,");
	LOG(" *  but an predicate can be provided also, if a custom comparison is required.");
	LOG(" *  Sorting the list is done not by and moving (and copying) of objects, but");
	LOG(" *  by changing the links. Therefore is more efficient to use the");
	LOG(" *  sort member function, than the template function std::sort.");
	LOG(" */");
	even.sort();
	log_to_console(even, "even.sort(); 	//even will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  splice function: Moves elements from one list into the another at the specified");
	LOG(" *  position. It inserts the specified elements into the destination container");
	LOG(" *  and removes them from the source container.");
	LOG(" */");

	int values1[] = {6, 3, 5};
	int numElem1 = sizeof(values1)/sizeof(values1[0]);

	int values2[] = {-11, 89 };
	int numElem2 = sizeof(values2)/sizeof(values2[0]);

	list<int> source(values1, values1 + numElem1 );
	list<int> destination(values2, values2 + numElem2);

	log_to_console(source, "\"source\" list contains: ");
	log_to_console(destination, "\"destination\" list contains: ");


	TRACE(list<int>::iterator position = destination.begin(););

	destination.splice(position, source);
	log_to_console(destination, "destination.splice(position, source); 	//destination will contain: ");

	LOG("\n");
}
