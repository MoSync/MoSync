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
 * @file  Employee.cpp
 * @author Gabriela Rata
 */

#include "Employee.h"

Employee::Employee():mExperience(0), mSalary(0)
{
}

Employee::Employee(int exp, const std::string &name):
	mExperience(exp),
	mName(name)
{
	if(mExperience > 0)
		mSalary = mExperience * 100;
	else
		mSalary = 30;

}

int Employee::getExperience() const
{
	return mExperience;
}

const std::string &Employee::getName() const
{
	return mName;
}

void Employee::setSalary(int val)
{
	mSalary = val;
}

int Employee::getSalary() const
{
	return mSalary;
}

//if two employees have equal salaries and experience, they compare equal
bool operator==(const Employee &lv, const Employee &rv)
{
	return  ( ( lv.getExperience() == rv.getExperience() ) &&
			(  lv.getSalary() == rv.getSalary() ) );
}

bool operator!=(const Employee &lv, const Employee &rv)
{
	return !( lv == rv );
}


bool lessExperience(const Employee &lv, const Employee &rv)
{
	return lv.getExperience() < rv.getExperience();
}

Employee &increaseSalary(Employee &e, int amount)
{
	int oldSalary = e.getSalary();
	e.setSalary(oldSalary + amount);
	return e;
}

bool equalSalary(const Employee &lv, const Employee &rv)
{
	return lv.getSalary() == rv.getSalary();
}
