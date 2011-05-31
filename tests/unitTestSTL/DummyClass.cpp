/*
 * DummyClass.cpp
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */


#include "DummyClass.h"
#include "LOG.h"

DummyClass::DummyClass(const std::string &name):mName(name)
{
	LOG("\t\tDummyClass constructor: %s", mName.c_str());
}
DummyClass::DummyClass(const DummyClass &rv)
{
	//LOG("\t\tDummyClass copy constructor");
	mName = rv.mName;
}
DummyClass &DummyClass::operator=(const DummyClass &rv)
{
	//LOG("\t\tDummyClass operator=");
	if(&rv != this)
	{
		mName = rv.mName;
	}
	return *this;
}
DummyClass::~DummyClass()
{
	LOG("\t\tDummyClass destructor: %s", mName.c_str());
}
void DummyClass::SomeFunction()
{
	LOG("\t\tDummyClass::SomeFunction");
}

std::string DummyClass::getName()const
{
	return mName;
}

bool operator==(const DummyClass &lv, const DummyClass &rv)
{
	return lv.mName==rv.mName;
}
