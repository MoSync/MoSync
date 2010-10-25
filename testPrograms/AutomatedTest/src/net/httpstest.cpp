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

using namespace Testify;
using namespace MAUtil;

class HTTPSTestCase : public TestCase
{
public:
        HTTPSTestCase( )
        : TestCase( "HTTPS test case" )
        {
			addTest( bind( &HTTPSTestCase::httpsConnectTest, this ), "HTTPS Connect test" );
			addTest( bind( &HTTPSTestCase::httpsReadTest, this ), "HTTPS Read test" );
        }

        void httpsConnectTest ( void )
        {
        	int result;
        	MAHandle sslConnection = maConnect("https://encrypted.google.com/");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT, result) );
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
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );

			maConnRead(sslConnection, buf, 2048);
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_READ, result) );
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

private:
        bool waitForEvent ( int ms, int opType )
        {
        	int result;
        	return waitForEvent ( ms, opType, result );
        }

        bool waitForEvent ( int ms, int opType, int &result )
        {
        	int iter = ms/100;
            MAEvent event;

            for ( int i = 0; i < iter; i++ )
            {
				maWait( 100 );
				while( maGetEvent( &event ) != 0 )
				{
					if ( event.type == EVENT_TYPE_CONN &&
					     event.conn.opType == opType )
					{
						result = event.conn.result;
						return true;
					}
				}
            }

            return false;
        }
};

static TestHook hook( new HTTPSTestCase( ), "net" );

