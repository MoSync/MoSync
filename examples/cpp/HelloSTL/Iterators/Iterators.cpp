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
 * @file Iterators.cpp
 * @author Gabriela Rata
 */

#include <vector>
#include <set>
#include <list>
#include "../STLMoblet.h"
#include "../LOG.h"

/**
*  An iterator is an object that can iterate through a range, pointing to the elements in the range.
*  An iterator has to provide at least operator++, to iterate forward the range. Also it has to
*  provide the dereference operator(*), so that we can access the object
*  to which it points.
*  All containers define their own iterators, which have different abilities.
*  Depending on the the abilities they have, iterators are classified in five categories:
*  		-> Input iterators
*  		-> Output iterators
*  		-> Forward iterators
*  		-> Bidirectional iterators
*  		-> Random access iterators.
*/

void STLMoblet::STL_iterators()
{
	LOG("========================= STL iterators ========================================================================");

	LOG("/**");
	LOG("*  Input iterators: allow only reading elements from a sequence and moving forward, one step,");
	LOG("*  using operator++. The elements are read with operator*.");
	LOG("*  Provides also operator== and operator!=.");
	LOG("*  An input iterator implemented in STL is for example the one provided by std::istream.");
	LOG("*/");
	LOG("\n");

	LOG("/**");
	LOG("*  Output iterators: allow only writing elements from a sequence and only moving forward, one step,");
	LOG("*  using operator++. They elements are written with operator*.");
	LOG("*  Provides also operator== and operator!=.");
	LOG("*  An input iterator implemented in STL is for example the one provided by std::ostream.");
	LOG("*/");
	LOG("\n");

	LOG("/**");
	LOG("*  Forward iterators: allow reading and writing elements from a sequence. Only moving forward, one step,");
	LOG("*  using operator++ is possible. They elements are read and written with operator*.");
	LOG("*  Provides also operator== and operator!=.");
	LOG("*  An input iterator implemented in STL is for example the one provided by std::ostream.");
	LOG("*/");

	LOG("/**");
	LOG("*  Bidirectional iterators: allow reading and writing elements from a sequence. Allows moving forward and backward,");
	LOG("*  one step,using operator++/operator--. Their elements are read and written with operator*.");
	LOG("*  Provides also operator== and operator!=.");
	LOG("*  An input iterator implemented in STL is for example the ones provided by list, multiset, map, multimap.");
	LOG("*/");
	LOG("\n");

	LOG("		   Example using iterators 	     		  ");

	int sArray[] = { 0, 11, 22, 33, 44 };
	int sArraySize = sizeof(sArray)/sizeof(sArray[0]);
	std::list<int> myList(sArray, sArray + sArraySize);

	LOG("Create a list object (myList) and fill it with: 0, 11, 22, 33, 44\n");

	LOG("\n");
	LOG("/**");
	LOG("*  create an bidirectional iterator");
	LOG("*/");

	LOG("std::list<int>::iterator iter;");
	std::list<int>::iterator iter;

	iter = myList.begin();
	LOG("iter = myList.begin();");

	LOG("\n");
	LOG("/**");
	LOG("*  read operation");
	LOG("*/");

	int r = *iter;
	log_to_console(r, "int r = *iter	//r = ");

	LOG("\n");
	LOG("/**");
	LOG("*  write operation");
	LOG("*/");

	*iter = 99;
	LOG("*iter = 99; //myList will contain now: 99, 11, 22, 33, 44");

	LOG("\n");
	LOG("/**");
	LOG("*  operator++");
	LOG("*/");

	iter++;
	log_to_console(*iter, "iter++;	//*iter is now: ");


	LOG("\n");
	LOG("/**");
	LOG("*  operator--");
	LOG("*/");

    --iter;
    log_to_console(*iter, "iter--;	//*iter is now: ");

    LOG("\n");
    LOG("/**");
    LOG("*  Random access iterators: allows all operations a normal pointer does: add and subtract integral values,");
    LOG("*  move forward and backward, one or more steps, use operator[] on it, subtract one iterator from another.");
    LOG("*  It overloads operator<, operator>, operator==, operator!=.");
    LOG("*  An input iterator implemented in STL is for example the ones provided by vector, deque, string.");
    LOG("*/");
    LOG("\n");

    LOG("Create vector object (myVector) and fill with: 0, 11, 22, 33, 44");
    std::vector<int> myVector(sArray, sArray + sArraySize);	//myVector contains now: 0, 11, 22, 33, 44


	LOG("\n");
	LOG("/**");
	LOG("*  create an random iterator");
	LOG("*/");

	std::vector<int>::iterator randomAcessIter;
	randomAcessIter = myVector.begin();

	LOG("std::vector<int>::iterator randomAcessIter;");
	LOG("randomAcessIter = myVector.begin();");

	LOG("\n");
	LOG("/**");
	LOG("*  read operation");
	LOG("*/");
	r = *randomAcessIter;
	log_to_console(r, "r = *randomAcessIter;	//r = ");

	LOG("\n");
	LOG("/**");
	LOG("*  write operation");
	LOG("*/");

	*randomAcessIter = 99;
	log_to_console(myVector, "*randomAcessIter = 99;//myVector will contain now: ");

	LOG("\n");
	LOG("/**");
	LOG("*  operator++");
	LOG("*/");

	randomAcessIter++;
	log_to_console(*randomAcessIter, "randomAcessIter++; //*randomAcessIter = ");

	LOG("\n");
	LOG("/**");
	LOG("*  operator--");
	LOG("*/");

	--randomAcessIter;
	log_to_console(*randomAcessIter, "randomAcessIter--; //*randomAcessIter = ");

	LOG("\n");
	LOG("/**");
	LOG("*  move two steps forward");
	LOG("*/");
	LOG("\n");

	randomAcessIter += 2;
	log_to_console(*randomAcessIter, "randomAcessIter +=2; //*randomAcessIter = ");

	LOG("\n");
}
