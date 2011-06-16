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
 * httpstest.cpp
 *
 *  Created on: 25 okt 2010
 *      Author: Ali Mosavian
 */
#include <conprint.h>
#include <mastring.h>
#include <MAUtil/Connection.h>
#include <Testify/testify.hpp>
#include "netbase.h"
#include "connection.h"

using namespace Testify;


/**
 * Automated HTTPS test case. Please feel free to adding
 * more cases to it.
 *
 */
class HTTPSTestCase : public TestCase, private NetBase
{
public:
        HTTPSTestCase( )
        : TestCase( "HTTPS test case" )
        {
			addTest( bind( &HTTPSTestCase::httpsConnectTest, this ), "HTTPS Connect test" );
			addTest( bind( &HTTPSTestCase::httpsReadTest, this ), "HTTPS Read test" );
        }


        virtual void testTearDown ( void )
        {
        	long startTime = maGetMilliSecondCount( );
        	while ( maGetMilliSecondCount()-startTime < 500 );
        }


        void httpsConnectTest ( void )
        {
        	int result;
        	Connection con( "https://encrypted.google.com/" );
			TESTIFY_ASSERT( con.isValid( ) == true );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_CONNECT, result ) );
#ifdef VERBOSE
			printf("@@@ httpsConnectTest maConnect result: %i\n", result);
#endif
			TESTIFY_ASSERT( result > 0 );
        }

        void httpsReadTest ( void )
        {
        	int result;
        	char buf[2048] = { 0 };

        	Connection con( "https://encrypted.google.com/" );
			TESTIFY_ASSERT( con.isValid( ) == true );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_CONNECT, result ) );
			TESTIFY_ASSERT( result > 0 );

			con.read( buf, 2048 );
			TESTIFY_ASSERT( waitForEvent( MAX_DELAY, CONNOP_READ, result ) );
			TESTIFY_ASSERT( result > 0 );

			// Ensure that we do not print too much data.
			if (result > 100)
			{
				result = 100;
			}
			buf[result] = 0;
#ifdef VERBOSE
			printf("Reply: %s\n", buf);
#endif
        }
};

static TestHook hook( new HTTPSTestCase( ), "net" );

