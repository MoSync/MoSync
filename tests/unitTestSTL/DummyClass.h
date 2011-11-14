/*
 * DummyClass.h
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */

#ifndef DUMMYCLASS_H_
#define DUMMYCLASS_H_

#include <string>

/**
 *  small helper class
 */
class DummyClass
{
public:
	DummyClass(const std::string &name);
	DummyClass(const DummyClass &rv);
	DummyClass &operator=(const DummyClass &rv);
	~DummyClass();
	void SomeFunction();
	std::string getName()const;
	friend bool operator==(const DummyClass &lv, const DummyClass &rv);
private:
	std::string mName;
};

bool operator==(const DummyClass &lv, const DummyClass &rv);


#endif /* DUMMYCLASS_H_ */
