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
 * @file Example_using_queue.cpp
 * @author Gabriela Rata
 */

#include <queue>
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"


void STLMoblet::STL_queue()
{
	LOG("\n");
	LOG("========================= queue =================================================================================\n");
	LOG("/**");
	LOG(" *The class queue implements a queue. With push(), you can insert elements.");
	LOG(" * With pop(), you can remove the elements in the same order in which they were inserted (FIFO - \"first in, first out\").");
	LOG(" */");
	LOG("\n");

	using std::queue;

	LOG("\n");
	LOG("/**");
	LOG("*  default constructor");
	LOG("*  Constructed with a std::vector as the underlying container and std::less as the comparison class.");
	LOG("*/");

	TRACE(queue<int> q;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the priority_queue contains no elements");
	LOG("*/");

	bool e = q.empty();
	log_to_console(e, "bool e = q.empty(); 	//e = ");

	LOG("\n");
	LOG("/**");
	LOG("* default constructor");
	LOG("* Constructed with a std::vector as the underlying container.");
	LOG("*/");
	TRACE(queue<Employee> q1;);

	LOG("\n");
	LOG("/**");
	LOG(" * push: Inserts a new element into the priority_queue. The new element is sorted into the queue according");
	LOG(" * to a comparison functor.");
	LOG(" * push calls the member function push_back of the underlying container object ( std::vector in our case ).");
	LOG(" */");

	LOG("\n");
	TRACE(Employee senior(8, "Alex"); );
	TRACE(Employee middle(4, "Sam"););
	TRACE(Employee junior(1, "John"););

	LOG("\n");
	TRACE(q1.push(senior););
	TRACE(q1.push(middle););
	TRACE(q1.push(junior);)

	LOG("\n");
	LOG("/**");
	LOG("* size function: returns the number of elements in the queue");
	LOG("*/");

	int sz = q1.size();
	log_to_console(sz, "int sz = q1.size();		//sz = ");

	LOG("\n");
	LOG("/**");
	LOG("* front function: returns a reference to the \"oldest\" element in the queue.");
	LOG("*/");

	Employee oldest = q1.front();
	log_to_console(oldest, "Employee oldest = q1.front(); 	//oldest = ");

	LOG("\n");
	LOG("/**");
	LOG("* back function: returns a reference to the \"newest\" element in the queue (last element pushed into the queue).");
	LOG("*/");

	Employee newest = q1.back();
	log_to_console(newest, "Employee newest = q1.back();	//newest = ");

	LOG("\n");
	LOG("/**");
	LOG("* pop function: removes the \"oldest\" element from the queue.");
	LOG("* queue::pop calls the member function pop_front of the underlying container object.");
	LOG("*/");

	LOG("\n");
	TRACE(q1.pop(););
	oldest = q1.front();
	log_to_console(oldest, "oldest = q1.front();	//oldest = ");

	LOG("\n");
	TRACE(q1.pop(););
	oldest = q1.front();
	log_to_console(oldest, "oldest = q1.front();	//oldest = ");

	LOG("\n");
}
