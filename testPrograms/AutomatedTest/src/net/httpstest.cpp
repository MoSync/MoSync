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
        	MAHandle sslConnection = maConnect("https://encrypted.google.com/");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_CONNECT, result) );
#ifdef VERBOSE
			printf("@@@ httpsConnectTest maConnect result: %i\n", result);
#endif
			TESTIFY_ASSERT( result > 0 );
			maConnClose(sslConnection);
        }

        void httpsReadTest ( void )
        {
        	int result;
        	char buf[2048] = { 0 };

        	MAHandle sslConnection = maConnect("https://encrypted.google.com/");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );

			maConnRead(sslConnection, buf, 2048);
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_READ, result) );
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

			maConnClose(sslConnection);
        }
};

static TestHook hook( new HTTPSTestCase( ), "net" );

