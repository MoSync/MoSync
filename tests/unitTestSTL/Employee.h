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

#endif /* EMPLOYEE_H_ */
