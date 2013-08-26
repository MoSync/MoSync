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
	Employee(int exp, const std::string &name = "");

	void setExperience(int exp);
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
