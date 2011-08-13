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

/*
 * InsertChildTest.cpp
 *
 * Contains general tests for maWidgetInsertChild.
 *
 *  Created on: Mar 29, 2011
 *      Author: fmattias
 */
#include <testify/testify.hpp>

#include <maapi.h>

#include <IX_WIDGET.h>

/**
 * Tests a valid insert at the beginning of list view.
 */
void testValidInsert()
{
	int listView = maWidgetCreate( MAW_LIST_VIEW );
	TESTIFY_ASSERT( listView > 0 );

	int firstChild = maWidgetCreate( MAW_LABEL );
	TESTIFY_ASSERT( firstChild > 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetInsertChild( listView, firstChild, 0 ), MAW_RES_OK );

	int secondChild = maWidgetCreate( MAW_LABEL );
	TESTIFY_ASSERT( secondChild > 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetInsertChild( listView, secondChild, 0 ), MAW_RES_OK );

	maWidgetDestroy( listView );
}

/**
 * Tests to insert an element at the end of a list.
 */
void testInsertLast()
{
	int listView = maWidgetCreate( MAW_LIST_VIEW );
	TESTIFY_ASSERT( listView > 0 );

	int firstChild = maWidgetCreate( MAW_LABEL );
	TESTIFY_ASSERT( firstChild > 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetInsertChild( listView, firstChild, 0 ), MAW_RES_OK );

	int secondChild = maWidgetCreate( MAW_LABEL );
	TESTIFY_ASSERT( secondChild > 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetInsertChild( listView, secondChild, -1 ), MAW_RES_OK );

	maWidgetDestroy( listView );
}

/**
 * Tests to insert an element at a bad index.
 */
void testBadInsert()
{
	int listView = maWidgetCreate( MAW_LIST_VIEW );
	TESTIFY_ASSERT( listView > 0 );

	int firstChild = maWidgetCreate( MAW_LABEL );
	TESTIFY_ASSERT( firstChild > 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetInsertChild( listView, firstChild, -515 ), MAW_RES_INVALID_INDEX );

	maWidgetDestroy( listView );
}

static Testify::Test *tests[] =
{
		Testify::Test::create( Testify::bind( testValidInsert ), "Testing insert at 0." ),
		Testify::Test::create( Testify::bind( testInsertLast ), "Testing insert last." ),
		Testify::Test::create( Testify::bind( testBadInsert ), "Testing bad insert." )
};
static Testify::TestHook hook( tests, "maWidgetInsertChild tests" );
