#include "testify/testify.hpp"
#include <MAUtil/Connection.h>
#include <conprint.h>

using namespace Testify;
using namespace MAUtil;

class SslTestCase : public TestCase
{
public:
        SslTestCase( )
        : TestCase( "SSLTestCase" )
        {
			addTest( bind( &SslTestCase::sslConnectTest, this ), "sslConnectTest" );
			addTest( bind( &SslTestCase::sslWriteTest, this ), "sslWriteTest" );
			addTest( bind( &SslTestCase::sslReadTest, this ), "sslReadTest" );
        }

        void sslConnectTest ( void )
        {
        	MAHandle sslConnection = maConnect("ssl://www.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT) );
			maConnClose(sslConnection);
        }

        void sslWriteTest ( void )
        {
        	const char *httpGetRequest = "GET / HTTP/1.1\r\n\r\n";
        	MAHandle sslConnection = maConnect("ssl://www.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT) );
			maConnWrite(sslConnection, httpGetRequest, sizeof(httpGetRequest));
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_WRITE) );
			maConnClose(sslConnection);
        }

        void sslReadTest ( void )
        {
        	char buf[2048] = { 0 };
        	const char *httpGetRequest = "GET / HTTP/1.0\r\n\r\n";
        	MAHandle sslConnection = maConnect("ssl://www.google.com:443");
			TESTIFY_ASSERT( sslConnection );
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_CONNECT) );

			maConnWrite(sslConnection, httpGetRequest, 22);
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_WRITE) );

			maConnRead(sslConnection, buf, 2048);
			TESTIFY_ASSERT( waitForEvent(5000, CONNOP_READ) );
			printf("Reply: %s\n", buf);
			maConnClose(sslConnection);
        }


private:
        bool waitForEvent ( int ms, int opType )
        {
        	int iter = ms/100;
            MAEvent event;

            for ( int i = 0; i < iter; i++ )
            {
				maWait(100);
				while(maGetEvent(&event))
				{
					if(event.type == EVENT_TYPE_CONN && event.conn.opType == opType) {
						return true;
					}
				}
            }

            return false;
        }
};

static TestHook hook( new SslTestCase( ) );
