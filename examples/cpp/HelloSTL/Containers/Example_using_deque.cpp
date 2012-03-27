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
 * @file Example_using_deque.cpp
 * @author Gabriela Rata
 */

#include <assert.h>
#include <deque>
#include "../STLMoblet.h"
#include "../LOG.h"

void STLMoblet::STL_deque()
{
	LOG("\n");
	LOG("========================= deque =================================================================================\n");
	LOG("/**");
	LOG(" * std::deque doesn't keep it's elements in a single block of memory (like vector). It uses multiple");
	LOG(" * blocks of memory and keeps track of them. Because of that, is fast on insertions at the end and also at the");
	LOG(" * beginning. Also it doesn't need to copy and destroy objects when it needs to allocate more memory.");
	LOG(" * Provides (reasonable fast) random access to its elements. It has a random access iterator. That means that we can iterate");
	LOG(" * through a vector step by step, using the ++ operator and --operator. Also we can move the iterator more then a step, and");
	LOG(" * have access to random positions like this:");
	LOG(" * 				++myVectorIterator; 	//ok. Move one step forward");
	LOG(" * 				--myVectorIterator; 	//ok. Move one step backward");
	LOG(" * 				myVectorIterator + 5; 	//ok. Move the iterator 5 positions forward.");
	LOG(" * 				myVectorIterator - 2	//ok");
	LOG(" * std::deque is defined in the <deque> header.");
	LOG(" */");

	using std::deque;

	LOG("\n");
	LOG("/**");
	LOG(" * default constructor");
	LOG(" */");
	TRACE(deque<int> d;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the deque contains no elements");
	LOG("*/");
	bool e = d.empty();
	log_to_console(e, "bool e = d.empty(); 	//e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * fills the deque with 3 elements (uninitialized ints).");
	LOG(" */");
	deque<int> d0(3);
	log_to_console(d0, "deque<int> d0(3); 	//d0 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * fills the deque vector with 4 ints (initialized with -1).");
	LOG(" */");
	deque<int> d1(4, -1);
	log_to_console(d1, "deque<int> d1(4, -1); 	//d1 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * constructs a vector from an array");
	LOG(" */");

	int array[]= {1, 2, 3, 4, 5};
	LOG("int array[]= {1, 2, 3, 4, 5};");

	TRACE(int numElements = sizeof(array)/sizeof(array[0]););
	deque<int> d2(array, array + numElements);
	log_to_console(d2, "deque<int> d2(array, array + numElements); 	//d2 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * copy constructor");
	LOG(" */");

	deque<int> d2_copy(d2);
	log_to_console(d2_copy, "deque<int> d2_copy(d2); 	//d2_copy will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator ==");
	LOG(" */");
	TRACE(assert( d2 == d2_copy););

	LOG("\n");
	LOG("/**");
	LOG(" * assignment operator");
	LOG(" */");

	deque<int> d3(10, -2);
	log_to_console(d3, "deque<int> d3(10, -2); 	//d3 will contain: ");

	d3 = d2;
	log_to_console(d3, "d3=d2; 	//d3 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * assign function");
	LOG(" * copies all the elements from d3 to d4.");
	LOG(" */");

	TRACE(deque<int> d4;);
	d4.assign(d3.begin(), d3.end());
	log_to_console(d3, "d4.assign(d3.begin(), d3.end());	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * clear function: all the elements in the vector are destroyed and removed from the vector,");
	LOG(" * after calling this function the vector will have the size zero.");
	LOG(" */");

	TRACE(d4.clear(););
	e = d4.empty();
	log_to_console(e, "e = d4.empty() 	//e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * overload version of the assign function");
	LOG(" * copies only the first 3 elements from d3 to d4");
	LOG(" */");

	d4.assign(d3.begin(), d3.begin() + 3);
	log_to_console(d4, "d4.assign(d3.begin(), d3.begin() + 3); 	//d4 contains: ");

	LOG("\n");
	LOG("/**");
	LOG(" * resize function: resizes the vector to contain the specified number of elements");
	LOG(" * prototype: void resize(size_t newSize, T obj = T())");
	LOG(" * if the specified size is smaller then the current size,");
	LOG(" * the content is reduced to the first \"newSize\" elements, and the rest are destroyed.");
	LOG(" * If the \"newSize\" is bigger than the current size, the deque is expanded by adding");
	LOG(" * at the end the number of objects needed. The objects are created by copy constructing after \"obj\".");
	LOG(" */");

	d4.resize(9);
	log_to_console(d4, "d4.resize(9); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * overload version of the assign function");
	LOG(" * fills the deque with 4 elements, having the value \"-68\"");
	LOG(" */");

	d4.assign(4, -68);
	log_to_console(d4, "d4.assign(4, -68); 	//d4 will contain:");

	LOG("\n");
	LOG("/**");
	LOG(" * push_back: adds an element at the end of the deque");
	LOG(" */");

	d4.push_back(99);
	log_to_console(d4, "d4.push_back(99);	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop_back: removes and destroys the last element of the vector (by resizing it by one)");
	LOG(" */");

	d4.pop_back();
	log_to_console(d4, "d4.pop_back(); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * push_front: adds an element at the beginning of the deque");
	LOG(" */");

	d4.push_front(57);
	log_to_console(d4, "d4.push_front(57); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop_front: removes and destroys the first element of the deque");
	LOG(" */");

	d4.pop_front();
	log_to_console(d4, "d4.pop_front(); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * begin: returns an iterator referring to the first element of the deque");
	LOG(" * If the container is empty, it will return the one past-the-end element in the container,");
	LOG(" * like the set::end() function. See bellow.");
	LOG(" */");
	TRACE(std::deque<int>::iterator itBegin = d4.begin(););

	LOG("\n");
	LOG("/**");
	LOG(" * end: returns an iterator referring to the one past the end element in the deque");
	LOG(" * We must not deference the iterator returned by end(). It is used to see is if we reached the");
	LOG(" * end of the container, when we iterate through it.");
	LOG(" */");

	TRACE(std::deque<int>::iterator itEnd = d4.end(););

	LOG("\n");
	LOG("//iterate through d4: ");
	TRACE(std::deque<int>::iterator iter;);

	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//adds 1 to each element of deque
		*iter += 1;
	}

	LOG("for(iter = itBegin; iter != itEnd; ++iter)");
	LOG("{");
	LOG("	*iter += 1;");
	LOG("}");
	log_to_console(d4, "//d4 contains now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * insert function: inserts an element before the specified position.");
	LOG(" * The position before it should insert the element is indicated by an iterator");
	LOG(" * inserts the value \"1001\", before the first element. The first element will be 1001,");
	LOG(" * after this function call.");
	LOG(" */");

	d4.insert(itBegin, 1001);
	log_to_console(d4, "d4.insert(itBegin, 1001); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * inserts before the second element the first 3 elements from d1.");
	LOG(" */");
	LOG("\n");

	log_to_console(d1, "//d1 contains: ");

	TRACE(itBegin = d4.begin(););
	d4.insert(itBegin + 1, d1.begin(), d1.begin() + 3);

	log_to_console(d4, "d4.insert(itBegin + 1, d1.begin(), d1.begin() + 3); 	"
			"//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * inserts 2 elements, with the value -93, before the first element of d4.");
	LOG(" */");

	TRACE(itBegin = d4.begin(););
	d4.insert(itBegin, 2, -93);
	log_to_console(d4, "d4.insert(itBegin, 2, -93); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" *  erase function: removes a single element, or a range of elements");
	LOG(" */");

	TRACE(itBegin = d4.begin(););
	d4.erase(itBegin, itBegin + 1);
	log_to_console(d4, "d4.erase(itBegin, itBegin + 1); 	//d4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator[]: returns a reference to the element at the specified position.");
	LOG(" */");

	int x = d4[0];
	log_to_console(x, "int x = d4[0]; 	//x = ");

	LOG("\n");
	LOG("/**");
	LOG(" * at: returns a reference to the element at the specified position.");
	LOG(" * same as operator[], with the difference that the \"at\" function will");
	LOG(" * throw an out_of_range exception, if the requested index is out of range.");
	LOG(" */");
	int x2 = d4.at(0);
	log_to_console(x2, "int x2 = d4.at(0); 	//x2 = ");

	LOG("\n");
	LOG("/**");
	LOG(" * front: returns a reference to the first element");
	LOG(" */");
	LOG("\n");

	int &a = d4.front();
	log_to_console(a, "int &a = d4.front(); 	//a = ");
	TRACE(a = 80;);

	int firstElem = d4.front();
	log_to_console(firstElem, "int firstElem = d4.front();	//firstElem = ");

	LOG("\n");
	LOG("/**");
	LOG(" * back: returns a reference to the last element");
	LOG(" */");

	int &b = d4.back();
	log_to_console(b, "int &b = d4.back(); 	//b = ");
	TRACE(b = 77;);

	int lastElem = d4.back();
	log_to_console(lastElem, "int lastElem = d4.front();	//lastElem = ");

	LOG("\n");
}
