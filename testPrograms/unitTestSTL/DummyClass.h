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
 * @file  DummyClass.h
 * @author Gabriela Rata
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
