/*
 * firsttest.cpp
 *
 *  Created on: 10 aug 2010
 *      Author: miki
 */
#include <testify/testify.hpp>
using namespace Testify;

void myTest1 ( int par1 )
{
    TESTIFY_ASSERT( par1 == 10 );
}

class MyTestCase : public TestCase
{
public:
    MyTestCase ( void )
    : TestCase( "Mickes Test case")
    {
        addTest( bind( &MyTestCase::myTestMethod, this ), "myTestMethod" );
    }

    void myTestMethod ( void )
    {

    }
};

Test *testArray[] =
{
    Test::create( bind( myTest1, 2 ), "myTest1" ),
    new MyTestCase( )
};

TestHook myhook( testArray, "mysuite" );
