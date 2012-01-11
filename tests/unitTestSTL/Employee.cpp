/*
 * Employee.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
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
