/* Copyright (C) 2009 Ali Mosavian

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/
#include "test.hpp"
#include "testfunction.hpp"

NAMESPACE_BEGIN( Testify )


/**
 * Creates a test function
 *
 * @param f Pointer to a functor which contains the test
 * @param n Test name, default value is 'no_name_function'
 */
Test * Test::create ( Functor *f,
					  const char *n )
{
	return new TestFunction( f, n );
}

NAMESPACE_END
