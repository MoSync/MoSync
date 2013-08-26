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

