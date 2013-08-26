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
 * @file  Test_auto_ptr.cpp
 * @author Gabriela Rata
 */

#include <memory>
#include <cassert>
#include "../STLMoblet.h"
#include "../DummyClass.h"
#include "../LOG.h"

/**
 *  Function illustrating the usage of std::auto_ptr
 */
void STLMoblet::STL_auto_ptr()
{
	LOG("\n");
	LOG("========================= example using auto_ptr ================================================================");
	LOG("/**");
	LOG(" * auto_ptr is a smart pointer that takes ownership of the pointer it stores.");
	LOG(" * It has a default constructor and a constructor that takes a pointer to its generic type.");
	LOG(" * When the auto_ptr object is destroyed, the pointer will be deleted.");
	LOG(" * The auto_ptr is defined in <memory> header");
	LOG(" */");
	LOG("\n");

	using std::auto_ptr;

	LOG("//start a scope");
	TRACE({);

		LOG("\n");
		LOG("/**");
		LOG(" * Default constructor");
		LOG(" */");
		LOG("\n");

		TRACE(std::auto_ptr<DummyClass> p_temp;);

		TRACE(p_temp = auto_ptr<DummyClass>(new DummyClass("p_temp")););

		LOG("\n");
		LOG("//p1 will go out of scope. DummyClass object will be destroyed\n");

	TRACE(});

	LOG("\n");
	LOG("/**");
	LOG(" * auto_ptr constructor");
	LOG(" */");
	TRACE(std::auto_ptr<DummyClass> p1(new DummyClass("p1")););

	LOG("\n");
	LOG("/**");
	LOG(" * reset function:");
	LOG(" * Takes ownership of the pointer provided as argument.");
	LOG(" * If the auto_ptr has ownership of a pointer, prior to the reset call, it will delete that pointer.");
	LOG(" */");
	LOG("\n");
	TRACE(DummyClass *pObj = new DummyClass("pObj"););

	LOG("\n");
	LOG("//deletes the stored pointer , and stores the pObj pointer");
	TRACE(p1.reset(pObj););

	LOG("\n");
	LOG("/**");
	LOG(" * get: returns the raw pointer that auto_ptr contains");
	LOG(" */");
	log_to_console((int)p1.get(), "p1.get() returns ");

	LOG("\n");
	LOG("/**");
	LOG(" * release: replaces the stored pointer with with a null pointer");
	LOG(" * and returns the previously stored pointer.");
	LOG(" */");
	LOG("\n");

	TRACE(DummyClass *pObj2 = p1.release(););
	log_to_console((int)p1.get(), "p1.get() returns ");

	LOG("\n");
	TRACE(delete pObj2);

	LOG("\n");
	LOG("/**");
	LOG(" *  operator =");
	LOG(" *  For two auto_ptr objects p1 and p2,");
	LOG(" *  p1 = p2, means that, p2 will release ownership of the pointer it stores, and p1 will own the pointer.");
	LOG(" *  At the same time p1 deletes his previously stored pointer.");
	LOG(" *  p2 will contain a NULL pointer.");
	LOG(" */");
	LOG("\n");

	LOG("\n");
	TRACE(p1.reset(new DummyClass("p1")));

	LOG("\n");
	TRACE(std::auto_ptr<DummyClass> p2(new DummyClass("p2")););

	LOG("\n");
	TRACE(p1 = p2;);

	LOG("\n");
	LOG("/**");
	LOG(" *  operator ->");
	LOG(" */");
	LOG("\n");

	TRACE(p1->SomeFunction(););

	LOG("\n");
	LOG("/**");
	LOG(" *  operator *");
	LOG(" */");
	LOG("\n");

	TRACE((*p1).SomeFunction(););

	LOG("\n");
}
