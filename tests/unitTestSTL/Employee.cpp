/*
 * Employee.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
 */

#include "Employee.h"

Employee::Employee(int exp, const std::string &name):
	mExperience(exp),
	mName(name)
{
	mSalary = mExperience * 100 + 50;
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
