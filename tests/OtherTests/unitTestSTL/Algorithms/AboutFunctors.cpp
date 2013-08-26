/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * FunctionObjects.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
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
