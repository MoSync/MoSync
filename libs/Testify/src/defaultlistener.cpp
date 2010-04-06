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
#ifndef MAPIP
#define MAPIP
#endif

#ifndef MAPIP
#include <stdio.h>
#else
#include <conprint.h>
#endif

#include "defaultlistener.hpp"

NAMESPACE_BEGIN( Testify )

/**
 * Constructor
 */
DefaultListener::DefaultListener ( void )
{
	m_indent[0] = NULL;
	m_nestLevel = 0;
}


/**
 * Notifies the listener that a test suite has started
 *
 * @param n Suite name
 */
void DefaultListener::beginSuite ( const char *n )
{
	printf( "\n%sSuite '%s' begin\n", m_indent, n );
	indentInc( );
}

/**
 * Notifies the listener that a test suite has ended
 *
 */
void DefaultListener::endSuite ( void )
{
	indentDec( );
	printf( "%sSuite end\n", m_indent );
}


/**
 * Notifies the listener that a test case has started
 *
 * @param n Suite name
 */
void DefaultListener::beginTestCase ( const char *n )
{
	printf( "\n%sTest case '%s' begin\n", m_indent, n );
	indentInc( );
}

/**
 * Notifies the listener that a test case has ended
 *
 */
void DefaultListener::endTestCase ( void )
{
	indentDec( );
	printf( "%sTest case end\n", m_indent );
}

/**
 * Notifies the listener that a test has started
 *
 * @param n Test name
 */
void DefaultListener::beginTest ( const char *n )
{
	m_testFailed = false;
	printf( "%sRunning test '%s'...", m_indent, n );
}

/**
 * Notifies the listener that a test has ended
 *
 */
void DefaultListener::endTest ( void )
{
	if ( m_testFailed == false )
		printf( "OK\n" );
}

/**
 * Called once before any tests have been run
 */
void DefaultListener::beginUnit ( void )
{
	printf( "%sTests begin\n", m_indent );
	indentInc( );
}

/**
 * Called after all tests (functions and suits) have
 * been run.
 *
 * @param t Number of tests run
 * @param f Number of tests failed
 */
void DefaultListener::endUnit ( int t, int f )
{
	indentDec( );
	printf( "%sTests end\n\n", m_indent );
	printf( "Ran %d tests with %d failures...\n", t, f );
}

/**
 * Notifies the listener that the test failed
 *
 * @param f File name
 * @param l Line in file
 * @param a Assertion that failed
 */
void DefaultListener::testFailed ( const char *f,
								   int l,
								   const char *a )
{
	m_testFailed = true;
	printf( "failed\n" );
	indentInc( );
	printf( "%s  - %s:%d - '%s'\n", m_indent, f, l, a );
	indentDec( );
}

/**
 * Internal, increase indentation level
 */
void DefaultListener::indentInc ( void )
{
	if ( m_nestLevel >= 31 )
		return;

	m_indent[m_nestLevel++] = ' ';
	m_indent[m_nestLevel] = NULL;
}

/**
 * Internal, decreases indentation level
 */

void DefaultListener::indentDec ( void )
{
	if ( m_nestLevel < 1 )
		return;

	m_nestLevel--;
	m_indent[m_nestLevel] = NULL;
}


NAMESPACE_END
