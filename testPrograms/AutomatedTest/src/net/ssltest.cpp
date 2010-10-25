#include <conprint.h>
#include <mastring.h>
#include <MAUtil/Connection.h>
#include <Testify/testify.hpp>

using namespace Testify;
using namespace MAUtil;

class SslTestCase : public TestCase
{
public:
        SslTestCase( )
        : TestCase( "SSL test case" )
        {
			addTest( bind( &SslTestCase::sslConnectTest, this ), "SSL Connect test" );
			addTest( bind( &SslTestCase::sslWriteTest, this ), "SSL Write test" );
			addTest( bind( &SslTestCase::sslReadTest, this ), "SSL Read test" );
        }

        void sslConnectTest ( void )
        {
        	int result;
        	MAHandle sslConnection = maConnect("ssl://encrypted.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT, result) );
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
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );
			maConnWrite(sslConnection, httpGetRequest, sizeof(httpGetRequest));
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_WRITE, result) );
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
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT, result) );
			TESTIFY_ASSERT( result > 0 );

			maConnWrite(sslConnection, httpGetRequest, 22);
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_WRITE, result) );
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

			// This string should be found at beginning of the reply.
			TESTIFY_ASSERT( strstr(buf, "HTTP/1.0 200 OK") == buf );

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

static TestHook hook( new SslTestCase( ), "net" );
