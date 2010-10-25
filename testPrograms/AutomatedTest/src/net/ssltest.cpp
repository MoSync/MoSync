#include <conprint.h>
#include <mastring.h>
#include <MAUtil/Connection.h>
#include <Testify/testify.hpp>
#include "netbase.h"

using namespace Testify;


/**
 * Automated SSL test case. Please feel free to adding
 * more cases to it.
 *
 * Note: This will most likely no work on j2me devices since
 *       it uses port 443 which seem to be prohibited for use.
 */
class SslTestCase : public TestCase, private NetBase
{
public:
        SslTestCase( )
        : TestCase( "SSL test case" )
        {
			addTest( bind( &SslTestCase::sslConnectTest, this ), "SSL Connect test" );
			addTest( bind( &SslTestCase::sslWriteTest, this ), "SSL Write test" );
			addTest( bind( &SslTestCase::sslReadTest, this ), "SSL Read test" );
        }

        virtual void testTearDown ( void )
        {
        	long startTime = maGetMilliSecondCount( );
        	while ( maGetMilliSecondCount()-startTime < 500 );
        }

        void sslConnectTest ( void )
        {
        	int result;
        	MAHandle sslConnection = maConnect("ssl://encrypted.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_CONNECT, result) );
#ifdef VERBOSE
			printf("@@@ sslConnectTest maConnect result: %i\n", result);
#endif
			TESTIFY_ASSERT( result > 0 );
			maConnClose(sslConnection);
        }

        void sslWriteTest ( void )
        {
        	int result;
        	const char *httpGetRequest = "GET / HTTP/1.1\r\n\r\n";
        	MAHandle sslConnection = maConnect("ssl://encrypted.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );
			maConnWrite(sslConnection, httpGetRequest, sizeof(httpGetRequest));
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_WRITE, result) );
			TESTIFY_ASSERT( result > 0 );
			maConnClose(sslConnection);
        }

        void sslReadTest ( void )
        {
        	int result;
        	char buf[2048] = { 0 };

        	const char *httpGetRequest = "GET / HTTP/1.0\r\n\r\n";
        	MAHandle sslConnection = maConnect("ssl://encrypted.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );

			maConnWrite(sslConnection, httpGetRequest, 22);
			TESTIFY_ASSERT( waitForEvent(MAX_DELAY, CONNOP_WRITE, result) );
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

			// This string should be found at beginning of the reply.
			TESTIFY_ASSERT( strstr(buf, "HTTP/1.0 200 OK") == buf );

			maConnClose(sslConnection);
        }
};

static TestHook hook( new SslTestCase( ), "net" );
