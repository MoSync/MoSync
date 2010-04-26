/* Copyright (C) 2010 Ali Mosavian

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
#include <maapi.h>
#include <conprint.h>
#include "idelistener.hpp"

NAMESPACE_BEGIN( Testify )

/**
 * Constructor
 */
IDEListener::IDEListener ( void )
{
}

void IDEListener::beginSuite ( const char *n )
{
	int len = sprintf( m_buffer, "__TEST_MARKUP__<suite name=\"%s\">", n );
	maWriteLog( m_buffer, len);
}

void IDEListener::endSuite ( void )
{
	int len = sprintf( m_buffer, "__TEST_MARKUP__</suite>" );
	maWriteLog( m_buffer, len );
}


/**
 * Notifies the listener that a test case has started
 *
 * @param n Suite name
 */
void IDEListener::beginTestCase ( const char *n )
{
	int len = sprintf( m_buffer, "__TEST_MARKUP__<suite name=\"%s\">\n", n );
	maWriteLog( m_buffer, len );
}

/**
 * Notifies the listener that a test case has ended
 *
 */
void IDEListener::endTestCase ( void )
{
	int len = sprintf( m_buffer, "__TEST_MARKUP__</suite>\n" );
	maWriteLog( m_buffer, len );
}

/**
 * Notifies the listener that a test has started
 *
 * @param n Test name
 */
void IDEListener::beginTest ( const char *n )
{
	m_testFailed = false;
	m_startTime = maGetMilliSecondCount();
	int len = sprintf( m_buffer, "__TEST_MARKUP__<test name=\"%s\">\n", n );
	maWriteLog( m_buffer, len );
}

/**
 * Notifies the listener that a test has ended
 *
 */
void IDEListener::endTest ( void )
{
	int elapsed = maGetMilliSecondCount() - m_startTime;
	int len = sprintf( m_buffer, "__TEST_MARKUP__<time elapsed=\"%d\"/></test>", elapsed );
	maWriteLog( m_buffer, len );
}

/**
 * Called once before any tests have been run
 */
void IDEListener::beginUnit ( void )
{
}

/**
 * Called after all tests (functions and suits) have
 * been run.
 *
 * @param t Number of tests run
 * @param f Number of tests failed
 */
void IDEListener::endUnit ( int t, int f )
{
}

/**
 * Notifies the listener that the test failed
 *
 * @param f File name
 * @param l Line in file
 * @param a Assertion that failed
 */
void IDEListener::testFailed ( const char *f,
								    int l,
								    const char *a )
{
        m_testFailed = true;
    	int len = sprintf( m_buffer, "__TEST_MARKUP__<![CDATA[failed:%s]]>\n", a );
    	maWriteLog( m_buffer, len );
    	len = sprintf(m_buffer, "__TEST_MARKUP__<location file=\"%s\" line=\"%d\"/>", f, l);
    	maWriteLog( m_buffer, len );
}



NAMESPACE_END
