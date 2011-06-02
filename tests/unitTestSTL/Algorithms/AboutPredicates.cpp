/*
 * Predicates.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
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

bool isEven(int x)
{
	return x%2 == 0;
}

bool isNegative(int x)
{
	return x < 0;
}

bool compareByExperience(const Employee &lv, const Employee &rv)
{
	return lv.getExperience() < rv.getExperience();
}

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

	int res = std::count_if(myVector.begin(), myVector.end(), isEven);		//returns the how many numbers are even
	TESTIFY_ASSERT(res == 6);

	res = std::count_if(myVector.begin(), myVector.end(), isNegative);		//returns how many numbers are negative
	TESTIFY_ASSERT( res == 0);

	//binary predicates:

	Employee staff[] = { Employee(3, "Jonny"), Employee(2, "Bob"), Employee(20, "Filomela")};

	int staffSize = sizeof(staff)/sizeof(staff[0]);
	std::vector<Employee> myStaff( staff, staff + staffSize);		//fill a vector with these employees

	/**
	* std::sort algorithm: sorts the elements, from a range, into ascending order
	*/

	//sort by experience
	std::sort(myStaff.begin(), myStaff.end(), compareByExperience); //supply a binary predicate for comparing employees

	TESTIFY_ASSERT( myStaff[0].getExperience() == 2);
	TESTIFY_ASSERT( myStaff[1].getExperience() == 3);
	TESTIFY_ASSERT( myStaff[2].getExperience() == 20);

}
