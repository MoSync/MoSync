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
 * @file FunctionObjects.cpp
 * @author Gabriela Rata
 */

#include "../TestSTL.h"
#include "../Employee.h"


/**
*  A functor is a class that can act like a function. It has the advantage that,
*  unlike functions, it can store data.
*  A functor is a class/struct that overloads the function call operator ("()"),
*  so that an instance of that class acts just like a function ( and can be
*  supplied were a function is expected ).
*
*  For example:
*  class MyFunctor
*  {
*  		public:
*  		int operator()()
*  		{
*			//some implementation
*			//returns a integer value
*  		}
*  		private:
*  			//some data
*  };
*
*  MyFunctor getNumber;
*  int someNumber = getNumber();		// MyFunctor::operator()() is called
*
*  The function call operator(that is "()"),  can be defined to take any number
*  of parameters, or no parameters at all.
*/

//example of a functor
class ComputeStaffCosts
{
public:
	ComputeStaffCosts(): mTotalCost(0){}

	int operator()(const Employee &rv)
	{
		mTotalCost += rv.getSalary();
	}

	int getResult() { return mTotalCost; }

private:
	int mTotalCost;
};

void TestSTL::example_functors()
{
	Employee array[] = { 	Employee(0, "Alex"),
							Employee(6, "John"),
							Employee(2, "Mary")
					   };

	int arraySize = sizeof(array)/sizeof(array[0]);

	ComputeStaffCosts staffCostFun;

	for(int i=0; i<arraySize; ++i)
	{
		staffCostFun(array[i]);
	}

	int currentMonthCost = staffCostFun.getResult();

	TESTIFY_ASSERT(currentMonthCost == (array[0].getSalary() +
										array[1].getSalary() +
										array[2].getSalary() ) );

	//hire another employee

	Employee newE(10, "Clara");
	staffCostFun(newE);

	int newCost = staffCostFun.getResult();

	TESTIFY_ASSERT(newCost > currentMonthCost);
}
