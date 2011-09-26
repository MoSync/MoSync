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
#include "../TestSTL.h"
#include "../DummyClass.h"
#include "../LOG.h"

/**
 * Function for testing std::auto_ptr
 * auto_ptr is a smart pointer that takes ownership of the pointer it stores.
 * When the auto_ptr object is destroyed, the pointer will be deleted.
 */
void TestSTL::test_auto_ptr()
{
	using std::auto_ptr;

	{
		/**
		 * Default constructor
		 */
		std::auto_ptr<DummyClass> p_temp;
		//p1 = new DummyClass(); 									//wrong. It won't compile
		p_temp = auto_ptr<DummyClass>(new DummyClass("p_temp")); 	//ok
	} 																//p1 goes out of scope. DummyClass object will be destroyed

	/**
	 * constructor
	 */
	std::auto_ptr<DummyClass> p1(new DummyClass("p1"));

	/**
	 * reset function:
	 * Takes ownership of the pointer provided as argument.
	 * If the auto_ptr has ownership of a pointer, prior to the reset call, it will delete that pointer.
	 *
	 */
	p1.reset(0);							//deletes the stored pointer, and stores the pObj pointer
	DummyClass *pObj = new DummyClass("pObj");
	p1.reset(pObj);

	/**
	 * get: returns the raw pointer that auto_ptr contains
	 */
	TESTIFY_ASSERT( p1.get() != 0 );

	/**
	 * release: replaces the stored pointer with with a null pointer
	 * and returns the previously stored pointer.
	 */
	DummyClass *pObj2 = p1.release();
	TESTIFY_ASSERT( p1.get() == 0 );
	delete pObj2;

	/**
	 *  operator =
	 *  For two auto_ptr objects p1 and p2,
	 *  p1 = p2, means that, p2 will release ownership of the pointer it stores, and p1 will own the pointer.
	 *  At the same time p1 deletes his previously stored pointer.
	 *  p2 will contain a NULL pointer.
	 */

	DummyClass *pP1 = new DummyClass("pP1");
	DummyClass *pP2 = new DummyClass("pP2");

	p1.reset(pP1);
	std::auto_ptr<DummyClass> p2;
	p2.reset(pP2);

	p1 = p2; 		//p1 deletes pP1; p1 stores now pP2; p2 contains a NULL pointer.


	/**
	 *  operator ->
	 */
	p1->SomeFunction();

	/**
	 *  operator *
	 */
	(*p1).SomeFunction();
}
