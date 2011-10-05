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
 * @file Example_using_priority_queue.cpp
 * @author Gabriela Rata
 */

#include <assert.h>
#include <queue>
#include <vector>
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"


void STLMoblet::STL_priority_queue()
{
	LOG("\n");
	LOG("========================= priority_queue ========================================================================\n");
	LOG("/**");
	LOG(" *  std::priority_queue is a container defined so that the top element is always the one with the highest value.");
	LOG(" *  std::priority_queue is implemented as a container adaptor. Containers adapters are classes that use an");
	LOG(" *  encapsulated container and provide a restricted interface to that container.");
	LOG(" *  The underlying container can be a STL container or some other  container type. It has to provide the");
	LOG(" *  following public member functions: back(), push_back(), pop_back().");
	LOG(" *  priority_queue is defined as a template taking three parameters:");
	LOG(" *  		template < 	class T,");
	LOG(" *  				class Container = vector<T>,");
	LOG(" *  				class Compare = less<typename Container::value_type>");
	LOG(" *  				>");
	LOG(" *  				class priority_queue;");
	LOG(" * 		The first parameter is the type of elements it will contain.");
	LOG(" * 		The second parameter is the underlying container, and is by default std::vector.");
	LOG(" * 		\"Compare\" is the comparison class. std::less is the default.");
	LOG(" * 		We can provide another comparison class. The class has to be a functor, with it's operator() taking two");
	LOG(" * 		arguments, of the same type as the container elements, and returning a bool. (See  example bellow ).");
	LOG(" * std::priority_queue is defined in the <queue> header.");
	LOG(" */");
	LOG("\n");


	using std::priority_queue;

	LOG("\n");
	LOG("/**");
	LOG("*  default constructor");
	LOG("*  Constructed with a std::vector as the underlying container and std::less as the comparison class.");
	LOG("*/");

	TRACE(priority_queue<int> pq;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the priority_queue contains no elements");
	LOG("*/");
	bool e = pq.empty();
	log_to_console(e, "bool e = pq.empty();	//e = ");

	LOG("\n");
	LOG("/**");
	LOG("*  constructor");
	LOG("*  Constructed with a std::vector as the underlying container and std::less as the comparison functor.");
	LOG("*  All the elements from the array will be copied into the pq1.");
	LOG("*/");
	LOG("\n");

	int array[] = { 4, 2, 6, 0, 8 };
	LOG("int array[] = { 4, 2, 6, 0, 8 };");

	TRACE(int arraySize = sizeof(array)/sizeof(array[0]););
	TRACE(priority_queue<int> pq1(array, array + arraySize););
	int highestElement = pq1.top();
	log_to_console(highestElement, "int highestElement = pq1.top();	//highestElement = ");

	LOG("\n");
	LOG("/**");
	LOG("* size function: returns the number of elements in the priority_queue");
	LOG("*/");

	int pq1Size = pq1.size();
	log_to_console(pq1Size, "int pq1Size = pq.size(); 	//pq1Size = ");

	LOG("\n");
	LOG("/**");
	LOG("* default constructor");
	LOG("* Constructed with a std::deque as the underlying container and LessExperienceFunctor as the comparison class.");
	LOG("*/");

	priority_queue<Employee, std::deque<Employee>, LessExperienceFunctor> pq2;
	LOG("priority_queue<Employee, std::deque<Employee>, LessExperienceFunctor> pq2;");

	LOG("\n");
	LOG("/**");
	LOG("* default constructor");
	LOG("* Constructed with a std::vector as the underlying container and LessExperienceFunctor as the comparison class.");
	LOG("*/");

	priority_queue<Employee, std::vector<Employee>, LessExperienceFunctor> pq3;
	LOG("priority_queue<Employee, std::vector<Employee>, LessExperienceFunctor> pq3;");

	LOG("\n");
	LOG("/**");
	LOG(" * push: Inserts a new element into the priority_queue. The new element is sorted into the queue according");
	LOG(" * to a comparison functor.");
	LOG(" * push calls the member function push_back of the underlying container object ( std::vector in our case).");
	LOG(" */");

	LOG("\n");
	LOG("/**");
	LOG("* top function: Returns a constant reference to the element that has the highest value.");
	LOG("*/");
	LOG("\n");

	TRACE(Employee junior(1, "John"););	//lowest priority
	TRACE(Employee middle(4, "Sam"););
	TRACE(Employee senior(8, "Alex"); ); //highest priority

	LOG("\n");
	TRACE(pq3.push(senior););
	Employee topVal = pq3.top();
	log_to_console(topVal, "Employee topVal = pq3.top(); 	//topVal = ");

	LOG("\n");
	TRACE(pq3.push(middle););
	topVal = pq3.top();
	log_to_console(topVal, "topVal = pq3.top(); 	//topVal = ");

	LOG("\n");
	TRACE(pq3.push(junior);)
	topVal = pq3.top();
	log_to_console(topVal, "topVal = pq3.top(); 	//topVal = ");

	LOG("\n");
}
