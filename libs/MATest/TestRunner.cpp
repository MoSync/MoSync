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
* @file TestRunner.cpp
* @brief High-level wrapper for defining and running test suites.
*/

#include <conprint.h>
#include "TestRunner.h"

namespace MATest
{

	// ********** Class TestRunner **********

	TestRunner* TestRunner::sInstance;

	TestRunner* TestRunner::getInstance()
	{
		if (NULL == TestRunner::sInstance)
		{
			TestRunner::sInstance = new TestRunner();
		}

		return TestRunner::sInstance;
	}

	TestRunner::TestRunner() :
		mSuite("TestRunner")
	{
	}

	TestRunner::~TestRunner()
	{
	}

	void TestRunner::addTestCase(TestCase* testCase)
	{
		mSuite.addTestCase(testCase);
	}

	void TestRunner::addTestListener(TestListener* listener)
	{
		mSuite.addTestListener(listener);
	}

	void TestRunner::runTests()
	{
		mSuite.runNextCase();
	}

	// ********** Class RegisterTestCase **********

	RegisterTestCase::RegisterTestCase(TestCase* testCase)
	{
		TestRunner::getInstance()->addTestCase(testCase);
	};

	// ********** Class HighLevelTestListener **********

	HighLevelTestListener::HighLevelTestListener()
	{
		reset();
	}

	HighLevelTestListener::~HighLevelTestListener()
	{
	}

	void HighLevelTestListener::beginTestSuite(
		const MAUtil::String& suiteName)
	{
	}

	void HighLevelTestListener::reset()
	{
		mNumTestCasesRan = 0;
		mNumAssertsPassed = 0;
		mTestCasesTimedOut.clear();
		mAssertsFailed.clear();
		mAssertsExpected.clear();
	}

	int HighLevelTestListener::getNumberOfTestCasesRan()
	{
		return mNumTestCasesRan;
	}

	int HighLevelTestListener::getNumberOfTestCasesTimedOut()
	{
		return (int) mTestCasesTimedOut.size();
	}

	MAUtil::String HighLevelTestListener::getTestCaseTimedOut(int index)
	{
		return mTestCasesTimedOut[index];
	}

	int HighLevelTestListener::getNumberOfAssertsPassed()
	{
		return mNumAssertsPassed;
	}

	int HighLevelTestListener::getNumberOfAssertsFailed()
	{
		return (int) mAssertsFailed.size();
	}

	MAUtil::String HighLevelTestListener::getAssertFailed(int index)
	{
		return mAssertsFailed[index];
	}

	int HighLevelTestListener::getNumberOfAssertsExpected()
	{
		return (int) mAssertsExpected.size();
	}

	MAUtil::String HighLevelTestListener::getAssertExpected(int index)
	{
		return mAssertsExpected[index];
	}

	// Standard error reporting that prints to the MoSync screen.
	// You can use this code as inspiration for your own listener class.
	// Just inherit HighLevelTestListener and override this method.
	void HighLevelTestListener::endTestSuite()
	{
		printf("Finished running tests\n");

		// Test cases.
		printf("Test cases ran: %i\n", getNumberOfTestCasesRan());
		printf("Test cases timed out: %i\n", getNumberOfTestCasesTimedOut());
		for (int i = 0; i < getNumberOfTestCasesTimedOut(); ++i)
		{
			printf("  %s\n", getTestCaseTimedOut(i).c_str());
		}

		// Asserts.
		printf("Asserts passed: %i\n", getNumberOfAssertsPassed());
		printf("Asserts failed: %i\n", getNumberOfAssertsFailed());
		for (int i = 0; i < getNumberOfAssertsFailed(); ++i)
		{
			printf("  %s\n", getAssertFailed(i).c_str());
		}

		// Expected asserts.
		printf("Expects failed: %i\n", getNumberOfAssertsExpected());
		for (int i = 0; i < getNumberOfAssertsExpected(); ++i)
		{
			printf("  %s\n", getAssertExpected(i).c_str());
		}
	}

	void HighLevelTestListener::beginTestCase(
		const MAUtil::String& testCaseName)
	{
		mCurrentTestCaseName = testCaseName;
		//printf("Running testcase: %s\n", testCaseName.c_str());
	}

	void HighLevelTestListener::endTestCase()
	{
		mNumTestCasesRan ++;
	}

	// Helper function for removing an entry from the expected list.
	void eraseExpected(
		MAUtil::Vector<MAUtil::String>& v,
		const MAUtil::String& s)
	{
		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] == s)
			{
				v.remove(i);
				return;
			}
		}
	}

	void HighLevelTestListener::assertion(
		const MAUtil::String& assertionName,
		bool cond)
	{
		MAUtil::String s = mCurrentTestCaseName + ":" + assertionName;
		if (!cond)
		{
			mAssertsFailed.add(s);
		}
		else
		{
			mNumAssertsPassed ++;
			eraseExpected(mAssertsExpected, s);
		}
	}

	void HighLevelTestListener::expectation(
		const MAUtil::String& assertionName)
	{
		MAUtil::String s = mCurrentTestCaseName + ":" + assertionName;
		mAssertsExpected.add(s);
	}

	void HighLevelTestListener::timedOut(
		const MAUtil::String& testCaseName)
	{
		mTestCasesTimedOut.add(testCaseName);
	}

} // namespace
