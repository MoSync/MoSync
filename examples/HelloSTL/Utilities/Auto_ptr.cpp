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
