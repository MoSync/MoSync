#include <ma.h>
#include <MATest/TestRunner.h>

/**
 * This is an example of a test case that
 * never calls runNextTestCase, and thus will
 * timeout. Note that you need to call setTimeOut
 * to specify the time out time. If you forget to
 * do that the test case will never exit.
 */
class TestThatWillTimeOut : public MATest::TestCase
{
public:
	TestThatWillTimeOut(const MAUtil::String& name)
		: MATest::TestCase(name)
	{
	}

	void start()
	{
		// Will timeout after two seconds.
		setTimeOut(2000);
	}
};

// Commented out test that works badly.
TESTCASE(TestThatWillTimeOut)
