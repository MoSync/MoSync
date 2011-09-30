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
 * @file  Employee.cpp
 * @author Gabriela Rata
 */

#include "Employee.h"
#include "LOG.h"

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

//	LOG("Created %s employee with experience = %d and salary = %d",
//	name.c_str(), mExperience, mSalary);
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
//	if(lv.getExperience() < rv.getExperience())
//	{
//		LOG("Calling binary predicate lessExperience(%s, %s). Returns true.",
//				lv.getName().c_str(), rv.getName().c_str());
//	}
//	else
//	{
//		LOG("Calling binary predicate lessExperience(%s, %s). Returns false.",
//						lv.getName().c_str(), rv.getName().c_str());
//	}
	return lv.getExperience() < rv.getExperience();
}

Employee &increaseSalary(Employee &e, int amount)
{
	int oldSalary = e.getSalary();
	e.setSalary(oldSalary + amount);
	LOG("Calling increaseSalary: oldSalary=%d,  newSalary = %d",
			oldSalary, oldSalary + amount);
	return e;
}

bool equalSalary(const Employee &lv, const Employee &rv)
{
	return lv.getSalary() == rv.getSalary();
}
