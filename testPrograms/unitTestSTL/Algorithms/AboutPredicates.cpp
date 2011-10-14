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
#include "../TestSTL.h"
#include "../Employee.h"
#include "../LOG.h"

/**
* A predicate is a function that returns a boolean value based on some comparison criterion.
* Predicates are used usually with STL algorithms.
* Binary predicates - predicates that take two arguments.
* Unary predicates - predicates taking one argument.
* Usually binary predicates are used with algorithms that need to compare two elements (e.g sorting algorithms).
*/


void TestSTL::predicate_example()
{

	// unary predicate example
	int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int sizeArray = sizeof(array)/sizeof(array[0]);
	std::vector<int> myVector(array, array + sizeArray); //fill a vector

	/**
	* std::countf_if algorithm: counts the number of elements that satisfy the
	* condition we supply (e.g isEven, isNegative)
	*/

	int res = std::count_if(myVector.begin(), myVector.end(), NumericUtilities::isEven);		//returns the how many numbers are even
	TESTIFY_ASSERT(res == 6);

	res = std::count_if(myVector.begin(), myVector.end(), NumericUtilities::isNegative);		//returns how many numbers are negative
	TESTIFY_ASSERT( res == 0);

	//binary predicates:

	Employee staff[] = { Employee(3, "Jonny"), Employee(2, "Bob"), Employee(20, "Filomela")};

	int staffSize = sizeof(staff)/sizeof(staff[0]);
	std::vector<Employee> myStaff( staff, staff + staffSize);		//fill a vector with these employees

	/**
	* std::sort algorithm: sorts the elements, from a range, into ascending order
	*/

	//sort by experience
	std::sort(myStaff.begin(), myStaff.end(), lessExperience); //supply a binary predicate for comparing employees

	TESTIFY_ASSERT( myStaff[0].getExperience() == 2);
	TESTIFY_ASSERT( myStaff[1].getExperience() == 3);
	TESTIFY_ASSERT( myStaff[2].getExperience() == 20);
}
