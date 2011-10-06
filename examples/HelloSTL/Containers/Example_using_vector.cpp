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
 * @file Example_using_vector.cpp
 * @author Gabriela Rata
 */

#include <vector>
#include <string>
#include <assert.h>
#include "../STLMoblet.h"


#include <vector>

#include "../STLMoblet.h"
#include "../LOG.h"

void STLMoblet::STL_vector()
{
	using std::vector;

	LOG("\n");
	LOG("========================= vector ================================================================================");
	LOG("/**");
	LOG(" * std::vector keeps its elements in a dynamic array and maintains its storage");
	LOG(" * as a single contiguous array of objects (in order to have efficient indexing and iteration).");
	LOG(" * A dynamic array is allocated first time the vector is constructed. When");
	LOG(" * vector needs a bigger array, to hold its elements,");
	LOG(" * it will allocate a new, bigger, chunk of memory and will copy all the");
	LOG(" * elements to this new chunk, deleting the old chunk and destroying the");
	LOG(" * objects contained in it.");
	LOG(" * std::vector provides (the fastest) random access to its elements. It has a");
	LOG(" * random access iterator. That means that we can iterate through a vector step");
	LOG(" * by step, using the ++ operator and --operator. And also we can move the");
	LOG(" * iterator more then a step, and have access to random positions like this:");
	LOG(" * 			++myVectorIterator; 	//ok. Move one step forward");
	LOG(" * 			--myVectorIterator; 	//ok. Move one step backward");
	LOG(" * 			myVectorIterator + 5;	//ok. Move the iterator 5 positions forward.");
	LOG(" * 			myVectorIterator - 2 	//ok");
	LOG(" * Has the fastest access speed of all STL containers. Also has the fastest");
	LOG(" * iterators.");
	LOG(" * Appending and removing elements at the end of the container is very fast.");
	LOG(" * Inserting an element at the middle or at the beginning, means that all the");
	LOG(" * following elements have to be moved to make room for the new element, so it");
	LOG(" * can take time.");
	LOG(" * std::vector is guaranteed to have the lowest space overhead per contained");
	LOG(" * object of all the STL containers(zero bytes).");
	LOG(" * It is the only STL container that guarantees that two object near each other");
	LOG(" * in container, will be near each other in memory.");
	LOG(" * The vector container is defined in <vector> header.");
	LOG(" */");
	LOG("\n");

	LOG("		example using vector\n");

	LOG("\n");
	LOG("/**");
	LOG(" * default constructor");
	LOG(" */");
	TRACE(vector<int> ve;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the vector contains no elements");
	LOG("*/");
	log_to_console(ve.empty(), std::string("ve.empty();	// returns "));

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * fills the vector with 3 elements (uninitialized ints).");
	LOG(" */");

	vector<int> v0(3);
	log_to_console(v0, "vector<int> v0(3);	//v0 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * fills the v1 vector with 3 ints (initialized with -1).");
	LOG(" */");

	vector<int> v1(3, -1);
	log_to_console(v1, "vector<int> v1(3, -1);	//v1 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * constructs a vector from an array");
	LOG(" */");

	int array[]= {1, 2, 3, 4, 5};
	LOG("int array[]= {1, 2, 3, 4, 5};");
	TRACE(int numElements = sizeof(array)/sizeof(array[0]););

	vector<int> v2(array, array + numElements);
	log_to_console(v2, "vector<int> v2(array, array + numElements);	//v2 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * copy constructor");
	LOG(" */");

	vector<int> v2_copy(v2);
	log_to_console(v2_copy, "vector<int> v2_copy(v2);	//v2_copy will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator ==");
	LOG(" */");
	TRACE(assert(v2 == v2_copy););

	LOG("\n");
	LOG("/**");
	LOG(" * assignment operator");
	LOG(" */");

	TRACE(vector<int> v3;);
	v3 = v2;
	log_to_console(v3, "v3 = v2;	//v3 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * assign function");
	LOG(" * copies all the elements from v3 to v4.");
	LOG(" */");

	TRACE(vector<int> v4;);
	v4.assign(v3.begin(), v3.end());
	log_to_console(v4, "v4.assign(v3.begin(), v3.end());	//v4 will contain: ");


	LOG("\n");
	LOG("/**");
	LOG(" * clear function: all the elements in the vector are destroyed and removed");
	LOG(" * from the vector.");
	LOG(" * after calling this function the vector will have the size zero.");
	LOG(" */");

	v4.clear();
	log_to_console((int)v4.size(), "v4.clear();	//v4.size() returns after this call ");

	LOG("\n");
	LOG("/**");
	LOG(" * overload version of the assign function");
	LOG(" * copies only the first 2 elements from v3 to v4");
	LOG(" */");

	v4.assign(v3.begin(), v3.begin() + 2);
	log_to_console(v4, "v4.assign(v3.begin(), v3.begin() + 2);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * resize function: resizes the vector to contain the specified number of");
	LOG(" * elements");
	LOG(" * prototype: void resize(size_t newSize, T obj = T())");
	LOG(" * if the specified size is smaller then the current size,");
	LOG(" * the content is reduced to the first \"newSize\" elements, and the rest are");
	LOG(" * destroyed.");
	LOG(" * If the \"newSize\" is bigger than the current size, the vector is expanded");
	LOG(" * by adding, at the end, the number of objects needed. The objects are");
	LOG(" * created by copy con6structing after \"c\".");
	LOG(" * resize doesn't affect the capacity of the vector");
	LOG(" */");

	v4.resize(1);
	log_to_console(v4, "v4.resize(1);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * overload version of the assign function");
	LOG(" * add to the vector one element, having the value \"-6\"");
	LOG(" */");

	v4.assign(1, -6);
	log_to_console(v4, "v4.assign(1, -6);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * push_back: adds an element at the end of the vector");
	LOG(" */");

	v4.push_back(99);
	log_to_console(v4, "v4.push_back(99);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop_back: removes and destroys the last element of the vector");
	LOG(" * (by resizing it by one)");
	LOG(" */");

	v4.pop_back();
	log_to_console(v4, "v4.pop_back();	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * begin: returns an iterator referring to the first element of the vector");
	LOG(" * If the container is empty, it will return the one past-the-end element");
	LOG(" * in the container. See bellow.");
	LOG(" */");

	std::vector<int>::iterator itBegin = v4.begin();
	log_to_console(*itBegin, "std::vector<int>::iterator itBegin = v4.begin();	"
			"//*itBegin = ");

	LOG("\n");
	LOG("/**");
	LOG(" * end: returns an iterator referring to the one past the end element in");
	LOG(" * the vector");
	LOG(" * We must not deference the iterator returned by vector::end(). It is used");
	LOG(" * to see is if we reached the end of the container, when we iterate");
	LOG(" * through it.");
	LOG(" */");

	TRACE(std::vector<int>::iterator iter;);
	std::vector<int>::iterator itEnd = v4.end();
	for(iter = itBegin; iter != itEnd; ++iter)
	{
		//adds 20 to each element of vector
		*iter += 20;
	}

	LOG("Iterate through v4 and add 20, to each element:");
	LOG("for(iter = itBegin; iter != itEnd; ++iter)");
	LOG("{");
	LOG("	//adds 20 to each element of vector");
	LOG("	*iter += 20;");
	LOG("}");

	log_to_console(v4, "//v4 contains now: ");


	LOG("\n");
	LOG("/**");
	LOG(" * insert function: inserts an element before the specified position.");
	LOG(" * The position before it should insert the element is indicated by an");
	LOG(" * iterator inserts the value \"1001\", before the first element. The first");
	LOG(" * element will be 1001, after this function call.");
	LOG(" */");

	v4.insert(itBegin, 1001);
	log_to_console(v4, "v4.insert(itBegin, 1001);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * inserts before the second element the first 3 elements from v1.");
	LOG(" */");

	v4.insert(itBegin + 1, v1.begin(), v1.begin() + 3);
	log_to_console(v4, "v4.insert(itBegin + 1, v1.begin(), v1.begin() + 3);	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * inserts 2 elements, with the value -90, before the first element of v4.");
	LOG(" */");

	v4.insert(itBegin, 2, -90);
	itBegin = v4.begin();

	log_to_console(v4, "v4.insert(itBegin, 2, -90); //v4 will contain: ");
	log_to_console(*itBegin, "itBegin = v4.begin();	//*itBegin = ");

	LOG("\n");
	LOG("/**");
	LOG(" *  erase function: removes a single element, or a range of elements");
	LOG(" */");

	v4.erase(itBegin, itBegin + 1);
	log_to_console(v4, "v4.erase(itBegin, itBegin + 1)	//v4 will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator[]: returns a reference to the element at the specified position.");
	LOG(" */");

	int x = v4[0];
	log_to_console(x, "int x = v4[0];	//x = ");

	LOG("\n");
	LOG("/**");
	LOG(" * at: returns a reference to the element at the spefified position.");
	LOG(" * same as operator[], with the difference that \"at\" function will");
	LOG(" * throw a out_of_range exception if the requested index is out of range.");
	LOG(" */");

	int x2 = v4.at(0);
	log_to_console(v4[0], "int x2 = v4.at(0);	//x = ");

	LOG("\n");
	LOG("/**");
	LOG(" * front: returns a reference to the first element");
	LOG(" */");

	int &a = v4.front();
	log_to_console(a, "int &a = v4.front();	//a = ");

	TRACE(a = 88;);
	TRACE(assert(v4.front() == 88););

	LOG("\n");
	LOG("/**");
	LOG(" * back: returns a reference to the last element");
	LOG(" */");

	int &b = v4.back();
	log_to_console(b, "int &b = v4.back();	//b = ");
	TRACE(b = -77;);
	TRACE(assert( v4.back() == -77););

	LOG("\n");
}
