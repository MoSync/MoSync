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
