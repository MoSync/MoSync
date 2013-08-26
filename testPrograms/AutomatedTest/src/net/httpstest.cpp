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

