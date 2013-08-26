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
* @file Test.h
* @brief Testing framework
*/

#include <conprint.h>
#include <matime.h>
#include <mavsprintf.h>
#include "Test.h"

namespace MATest
{
	/* ========== Class TestListener ========== */

	TestListener::TestListener() {}
	TestListener::~TestListener() {}
	void TestListener::beginTestSuite(const MAUtil::String& suiteName) {}
	void TestListener::endTestSuite() {}
	void TestListener::beginTestCase(const MAUtil::String& testCaseName) {}
	void TestListener::endTestCase() {}
	void TestListener::assertion(const MAUtil::String& assertionName, bool cond) {}
	void TestListener::expectation(const MAUtil::String& assertionName) {}
	void TestListener::timedOut(const MAUtil::String& testCaseName) {}

	/* ========== Class TestCaseTimeOutListener ========== */

	void TestCaseTimeOutListener::setTestCase(TestCase* testCase)
	{
		mTestCase = testCase;
	}

	void TestCaseTimeOutListener::startTimer(int ms)
	{
		stopTimer();
		MAUtil::Environment::getEnvironment().addTimer(this, ms, 1);
	}

	void TestCaseTimeOutListener::stopTimer()
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}

	void TestCaseTimeOutListener::runTimerEvent()
	{
		// The test case has timed out.
		stopTimer();
		mTestCase->timeOut();
	}

	/* ========== Class TestCase ========== */

	TestCase::TestCase(const MAUtil::String& name) :
		name(name)
	{
		mTimeOutListener.setTestCase(this);
	}

	TestCase::~TestCase()
	{
	}

	void TestCase::setTimeOut(int ms)
	{
		mTimeOutListener.startTimer(ms);
	}

	void TestCase::clearTimeOut()
	{
		mTimeOutListener.stopTimer();
	}

	void TestCase::open()
	{
	}

	void TestCase::close()
	{
	}

	bool TestCase::assert(const MAUtil::String& assertionName, bool cond)
	{
		suite->fireAssertion(assertionName, cond);
		return cond;
	}

	void TestCase::expect(const MAUtil::String& assertionName)
	{
		suite->fireExpectation(assertionName);
	}

	void TestCase::timeOut()
	{
		suite->fireTimedOut(name);
		runNextTestCase();
	}

	MAUtil::String TestCase::getName() const
	{
		return name;
	}

	void TestCase::setSuite(TestSuite *suite)
	{
		this->suite = suite;
	}

	TestSuite* TestCase::getSuite()
	{
		return this->suite;
	}

	void TestCase::runNextTestCase()
	{
		getSuite()->runNextCase();
	}

	/* ========== Class TestSuite ========== */

	TestSuite::TestSuite(const MAUtil::String& name) :
		mName(name),
		mCurrentTestCase(-1),
		mRunCounter(0)
	{
	}

	TestSuite::~TestSuite()
	{
	}

	void TestSuite::addTestCase(TestCase* testCase)
	{
		mTestCases.add(testCase);
		testCase->setSuite(this);
	}

// TODO: Why is this commented out?
// This code seems to run all test cases sequentially.
#if 0
	void TestSuite::runTestCases() {
		for(int i = 0; i < mTestListeners.size(); i++) {
			mTestListeners[i]->beginTestSuite(mName);
		}
		for(int i = 0; i < mTestCases.size(); i++) {
			for(int i = 0; i < mTestListeners.size(); i++) {
				mTestListeners[i]->beginTestCase(mTestCases[i]->getName());
			}
			mTestCases[i]->open();
			mTestCases[i]->run();
			mTestCases[i]->close();
			for(int i = 0; i < mTestListeners.size(); i++) {
				mTestListeners[i]->endTestCase();
			}
		}
		for(int i = 0; i < mTestListeners.size(); i++) {
			mTestListeners[i]->endTestSuite();
		}
	}
#endif

#if(0)
	// TODO: Should we use this rather than resetting
	// mCurrentTestCase in runNextCaseHelper()?
	void TestSuite::reset()
	{
		mCurrentTestCase = -1;
	}
#endif

	// Note: This method is called from TestCase::runNextTestCase().
	void TestSuite::runNextCase()
	{
		// If this the number of waiting test cases is zero
		// we launch a timer to run the next test case.
		if (0 == mRunCounter)
		{
			MAUtil::Environment::getEnvironment().addTimer(this, 1, 1);
		}

		// Increment counter for test cases to run.
		mRunCounter ++;
	}

	void TestSuite::runTimerEvent()
	{
		// Remove the timer.
		MAUtil::Environment::getEnvironment().removeTimer(this);

		// Run next test case.
		runNextCaseHelper();

		// Decrement run counter.
		mRunCounter --;

		// If there are test cases left to run, start a timer
		// to run it/them (should likely be only one waiting).
		if (mRunCounter > 0)
		{
			MAUtil::Environment::getEnvironment().addTimer(this, 1, 1);
		}
	}

	void TestSuite::runNextCaseHelper()
	{
		TestCase* testCase;

		// There must be test cases to run.
		if (mTestCases.size() > 0)
		{
			// Increment test case index.
			mCurrentTestCase ++;

			// Is this the first test case?
			if (0 == mCurrentTestCase)
			{
				// Signal beginning of the test suite.
				fireBeginTestSuite(mName);
			}
			// If this is not the first test case then
			// signal the end of the previous test case.
			else
			{
				// Close/clear the previous test case.
				testCase = mTestCases[mCurrentTestCase - 1];
				testCase->clearTimeOut();
				testCase->close();
				fireEndTestCase();
			}

			// If last test case has been run, reset the
			// test case index and end the suite.
			if (mCurrentTestCase >= mTestCases.size())
			{
				mCurrentTestCase = -1;
				fireEndTestSuite();
				return;
			}

			// Get the current test case.
			testCase = mTestCases[mCurrentTestCase];

			// Open the current test case.
			fireBeginTestCase(testCase->getName());
			testCase->open();

			// Run current test case.
			testCase->start();

			// Note: We do not call fireEndTestCase() here,
			// because it should not be called until the
			// current test case has called runNextTestCase().
		}
	}

	const MAUtil::String& TestSuite::getName() const
	{
		return mName;
	}

	void TestSuite::addTestListener(TestListener* testListener)
	{
		mTestListeners.add(testListener);
	}

	void TestSuite::fireBeginTestSuite(const MAUtil::String& suiteName)
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->beginTestSuite(suiteName);
		}
	}

	void TestSuite::fireEndTestSuite()
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->endTestSuite();
		}
	}

	void TestSuite::fireBeginTestCase(const MAUtil::String& testCaseName)
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->beginTestCase(testCaseName);
		}
	}

	void TestSuite::fireEndTestCase()
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->endTestCase();
		}
	}

	void TestSuite::fireAssertion(const MAUtil::String& assertionName, bool cond)
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->assertion(assertionName, cond);
		}
	}

	void TestSuite::fireExpectation(const MAUtil::String& assertionName)
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->expectation(assertionName);
		}
	}

	void TestSuite::fireTimedOut(const MAUtil::String& testCaseName)
	{
		for (int i = 0; i < mTestListeners.size(); i++)
		{
			mTestListeners[i]->timedOut(testCaseName);
		}
	}
}
// namespace
