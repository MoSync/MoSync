/*
 * Employee.h
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
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

bool lessExperience(const Employee &lv, const Employee &rv);
bool equalSalary(const Employee &lv, const Employee &rv);

Employee &increaseSalary(Employee &e, int amount = 100);


#endif /* EMPLOYEE_H_ */
