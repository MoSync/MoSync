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
 * @file  DummyClass.cpp
 * @author Gabriela Rata
 */


#include "DummyClass.h"
#include "LOG.h"

DummyClass::DummyClass(const std::string &name):mName(name)
{
}
DummyClass::DummyClass(const DummyClass &rv)
{
	mName = rv.mName;
}
DummyClass &DummyClass::operator=(const DummyClass &rv)
{
	if(&rv != this)
	{
		mName = rv.mName;
	}
	return *this;
}
DummyClass::~DummyClass()
{
}
void DummyClass::SomeFunction()
{
}

std::string DummyClass::getName()const
{
	return mName;
}

bool operator==(const DummyClass &lv, const DummyClass &rv)
{
	return lv.mName==rv.mName;
}
