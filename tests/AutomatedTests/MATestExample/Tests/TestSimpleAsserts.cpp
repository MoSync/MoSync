#include <MATest/TestRunner.h>

/**
 * This is just a very simple example of
 * a test case that demonstrates the use
 * of plain asserts (no async calls etc).
 */
class TestSimpleAsserts : public MATest::TestCase
{
public:
	TestSimpleAsserts(const MAUtil::String& name)
		: MATest::TestCase(name)
	{
	}

	void start()
	{
		testAddition();
		testSubtraction();
		testFailure();

		runNextTestCase();
	}

	void testAddition()
	{
		assert("Add two numbers", 1+1==2);
		assert("Add three numbers", 1+1+1==3);
	}

	void testSubtraction()
	{
		assert("Substract two numbers", 1-1==0);
		assert("Substract three numbers", 1-1-1==-1);
	}

	void testFailure()
	{
		assert("Test supposed to fail", false);
	}
};

TESTCASE(TestSimpleAsserts)

// Expands to:
//   static MATest::TestCase* sTestCase =
//       new TestSimpleAsserts("TestSimpleAsserts");
//   static RegisterTestCase __TestSimpleAsserts(sTestCase);

