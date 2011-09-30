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
		mTotalCost += rv.getSalary();
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
