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
 * @file Predicates.cpp
 * @author Gabriela Rata
 */

#include <algorithm>
#include <vector>
#include <string>
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"

/**
* Function for illustrating predicates
*/
void STLMoblet::predicates_explained()
{
	LOG("\n");
	LOG("========================= about predicates ======================================================================");
	LOG("/**");
	LOG("* A predicate is a function that returns a boolean value based on some");
	LOG("* comparison criterion.");
	LOG("* Predicates are used usually with STL algorithms.");
	LOG("* Binary predicates - predicates that take two arguments.");
	LOG("* Unary predicates - predicates taking one argument.");
	LOG("* Usually binary predicates are used with algorithms that need to compare two");
	LOG("* elements (e.g sorting algorithms).");
	LOG("*/");
	LOG("\n");

	LOG("				Example using predicates: 						  ");

	LOG("\n    Unary predicate example...");

	int array[] = { 0, 1, 2, 3, 4};
	int sizeArray = sizeof(array)/sizeof(array[0]);
	std::vector<int> myVector(array, array + sizeArray); //fill a vector

	log_to_console(myVector, "myVector contains: ");

	LOG("/**");
	LOG("* std::countf_if algorithm: counts the number of elements that satisfy the");
	LOG("* condition we supply (e.g isEven, isNegative)");
	LOG("*/");

	log_to_console("Calling: std::count_if(myVector.begin(), myVector.end(), "
			"isEven);");

	int res = std::count_if(myVector.begin(), myVector.end(),
			NumericUtilities::isEven);	//returns the how many numbers are even

	log_to_console(res, "int res = std::count_if(myVector.begin(), myVector.end(), "
			"isEven); //res =  ");

	log_to_console("\n    Binary predicate example...");

	Employee staff[] = { 	Employee(3, "Jonny"),
							Employee(2, "Bob"),
							Employee(20, "Filomela")};

	int staffSize = sizeof(staff)/sizeof(staff[0]);

	std::vector<Employee> myStaff( staff, staff + staffSize);
	log_to_console(myStaff, "myStaff contains: ");

	LOG("\n");
	LOG("/**");
	LOG("* std::sort algorithm: sorts the elements, from a range, into ascending");
	LOG("* order");
	LOG("* lessExperience - a binary predicate for comparing employees");
	LOG("*/");
	LOG("\n");

	TRACE(std::sort(myStaff.begin(), myStaff.end(), lessExperience));;

	log_to_console(myStaff, "myStaff contains now: ");

	LOG("\n");
}
