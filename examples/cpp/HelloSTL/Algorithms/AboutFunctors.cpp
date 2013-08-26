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

/**
 * @file AboutFunctors.cpp
 * @author Gabriela Rata
 */

#include <assert.h>
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"



class ComputeStaffCosts
{
public:
	ComputeStaffCosts(): mTotalCost(0)
	{
	}

	int operator()(const Employee &rv)
	{
		return mTotalCost += rv.getSalary();
	}

	int getResult()
	{
		return mTotalCost;
	}

private:
	int mTotalCost;
};

/**
*  Function for illustrating the use of functors
*/
void STLMoblet::functors_explained()
{
	LOG("\n");
	LOG("========================= about functors ========================================================================");
	LOG("/**"																				);
	LOG("*  A functor is a class that can act like a function. It has the advantage that,"	);
	LOG("*  unlike functions, it can store data."											);
	LOG("*  A functor is a class/struct that overloads the function call operator (\"()\"),");
	LOG("*  so that an instance of that class acts just like a function ( and can be"		);
	LOG("*  supplied were a function is expected )."										);
	LOG("*"																					);
	LOG("*  For example:"																	);
	LOG("*  class MyFunctor"																);
	LOG("*  {"																				);
	LOG("*  	public:"																);
	LOG("*  	int operator()()"														);
	LOG("*  	{"																		);
	LOG("*			//some implementation"												);
	LOG("*			//returns a integer value"											);
	LOG("*  	}"																		);
	LOG("*  	private:"																);
	LOG("*  		//some data"														);
	LOG("*  };"																				);
	LOG("*"																					);
	LOG("*  MyFunctor getNumber;"															);
	LOG("*  int someNumber = getNumber();		// MyFunctor::operator()() is called"		);
	LOG("*"																					);
	LOG("*  The function call operator(that is \"()\"),  can be defined to take any number"	);
	LOG("*  of parameters, or no parameters at all."										);
	LOG("*/"																				);
	LOG("\n"																				);
	LOG("\n"																				);

	LOG("			Example using functors 							  "						);
	LOG("\n");

	LOG("Define the ComputeStaffCosts functor:\n");
	LOG("class ComputeStaffCosts");
	LOG("{");
	LOG("public:");
	LOG("	ComputeStaffCosts(): mTotalCost(0) {}");
	LOG("\n");
	LOG("	int operator()(const Employee &rv)");
	LOG("	{");
	LOG("		mTotalCost += rv.getSalary();");
	LOG("	}");
	LOG("\n");
	LOG("	int getResult()");
	LOG("	{");
	LOG("		return mTotalCost;");
	LOG("	}");
	LOG("\n");
	LOG("private:");
	LOG("	int mTotalCost;");
	LOG("};");
	LOG("\n");

	Employee staff[] = { 	Employee(0, "Alex"), Employee(6, "John"), Employee(2, "Mary")};
	log_to_console("Employee staff[] = { Employee(0, \"Alex\"), Employee(6, \"John\"), Employee(2, \"Mary\")};\n");

	int staffSize = sizeof(staff)/sizeof(staff[0]);

	TRACE(ComputeStaffCosts staffCostFun;);
	log_to_console("\n");

	for(int i=0; i<staffSize; ++i)
	{
		LOG("staffCostFun(staff[%d]);	//calls ComputeStaffCosts::operator()", i);
		staffCostFun(staff[i]);	//calls ComputeStaffCosts::operator()
		LOG("\n");
	}

	int currentMonthCost = staffCostFun.getResult();
	log_to_console(currentMonthCost, "int currentMonthCost = "
			"staffCostFun.getResult();	//currentMonthCost = ");

	LOG("\n");
	TRACE(Employee newE(10, "Clara"););
	staffCostFun(newE);
	log_to_console("staffCostFun(newE);	//calls ComputeStaffCosts::operator()");

	int newCost = staffCostFun.getResult();
	log_to_console(newCost, "\n    int newCost = "
			"staffCostFun.getResult();	//newCost = ");

	assert(newCost > currentMonthCost);

	LOG("\n");
}
