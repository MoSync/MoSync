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
 * @file  Employee.h
 * @author Gabriela Rata
 */

#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <string>

/**
* small helper class
*/
class Employee
{
public:
	Employee();
	Employee(int exp, const std::string &name);

	int getExperience() const;
	const std::string &getName() const;

	void setSalary(int val);
	int getSalary() const;

private:
	int 			mExperience;
	int 			mSalary;
	std::string 	mName;
};

bool operator==(const Employee &lv, const Employee &rv);
bool operator!=(const Employee &lv, const Employee &rv);

bool lessExperiencePredicate(const Employee &lv, const Employee &rv);
bool equalSalary(const Employee &lv, const Employee &rv);

Employee &increaseSalary(Employee &e, int amount = 100);

class LessExperienceFunctor
{
public:
	bool operator()(const Employee &e1, const Employee &e2) const;
};


#endif /* EMPLOYEE_H_ */
