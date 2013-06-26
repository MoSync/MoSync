/*
Copyright (C) 2013 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
* @file Test.h
* @brief Testing framework
*/

#ifndef __MOSYNC_LIBS_MATEST_TEST_H__
#define __MOSYNC_LIBS_MATEST_TEST_H__

#include <ma.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include <MAUtil/Environment.h>

namespace MATest
{
	// Forward declarations.
	class TestSuite;
	class TestCase;

	/**
	* @brief Listener for events triggered when running tests.
	*/
	class TestListener
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestListener();

		/**
		 * @brief Destructor.
		 */
		virtual ~TestListener();

		/**
		 * @brief Called before start of a test suite.
		 */
		virtual void beginTestSuite(const MAUtil::String& suiteName);

		/**
		 * @brief Called after end of a test suite.
		 */
		virtual void endTestSuite();

		/**
		 * @brief Called before start of a test case.
		 */
		virtual void beginTestCase(const MAUtil::String& testCaseName);

		/**
		 * @brief Called after end of a test case.
		 */
		virtual void endTestCase();

		/**
		 * @brief Called when an assert is made.
		 */
		virtual void assertion(const MAUtil::String& assertName, bool cond);

		/**
		 * @brief Called when an expect is made.
		 * An expect says that "this assert is
		 * expected to happen in the future".
		 */
		virtual void expectation(const MAUtil::String& assertName);

		/**
		 * @brief Called when a test case times out.
		 */
		virtual void timedOut(const MAUtil::String& testCaseName);
	};

	/**
	* @brief Class that handles timeout of test cases.
	*/
	class TestCaseTimeOutListener : public MAUtil::TimerListener
	{
	public:

		/**
		 * @brief Set the test case of this listener.
		 */
		void setTestCase(TestCase* testCase);

		/**
		 * @brief Start the timer with the given the timeout value.
		 */
		void startTimer(int ms);

		/**
		 * @brief Stop timeout timer.
		 */
		void stopTimer();

		/**
		 * @brief Called on test case timeout.
		 */
		virtual void runTimerEvent();

		/**
		 * @brief The test case of this listener.
		 */
		TestCase* mTestCase;
	};

	/**
	* @brief Base class for test cases.
	*
	* Override open and close to initialize and clean up the
	* test case as needed. Override start to perform actual tests.
	* Use assert state conditions in the test that should hold.
	*
	* Do not initialize any non-const variables in the constructor,
	* as the test case might be run more than once.
	*/
	class TestCase
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestCase(const MAUtil::String& name);

		/**
		 * @brief Destructor.
		 */
		virtual ~TestCase();

		/**
		 * @brief Set the timeout for the test case.
		 * Starts the timeout timer with this delay.
		 */
		virtual void setTimeOut(int ms);

		/**
		 * @brief Stop the timeout timer.
		 */
		virtual void clearTimeOut();

		/**
		 * @brief Called before a test case is run.
		 * Do initialisation here as needed.
		 */
		virtual void open();

		/**
		 * @brief Called after a test case is run.
		 * Do clean up here as needed.
		 */
		virtual void close();

		/**
		 * @brief Called to run a test case.
		 * You must override this method and
		 * perform tests in a subclass.
		 */
		virtual void start() = 0;

		/**
		 * @brief State a condition that should hold.
		 */
		virtual bool assert(const MAUtil::String& assertionName, bool success);

		/**
		 * @brief State that an assert is expected to happen
		 * in the future.
		 */
		virtual void expect(const MAUtil::String& assertionName);

		/**
		 * @brief State that this test case has timed out.
		 * Stops running the test case.
		 */
		virtual void timeOut();

		/**
		 * @brief Call to run the next test case. This must
		 * be called at the end of each test case, during the
		 * invocation of the start method.
		 */
		virtual void runNextTestCase();

		/**
		 * @brief Get the name of this test case.
		 */
		virtual MAUtil::String getName() const;

		/**
		 * @brief Get the test suite this test case belongs to.
		 */
		virtual TestSuite* getSuite();

		/**
		 * @brief Set the test suite of this test case.
		 * You normally do not call this method explicitly,
		 * it is used by the test framework.
		 */
		virtual void setSuite(TestSuite* suite);

	protected:
		/**
		 * @brief @deprecated Do not access this variable in your code!
		 * Use getSuite() instead.
		 * TODO: Rename to mSuite.
		 */
		TestSuite* suite;

		/**
		 * @brief @deprecated Do not access this variable in your code!
		 * Use getName() instead.
		 * TODO: Rename to mName.
		 */
		MAUtil::String name;

		/**
		 * @brief TimerListener for test timeout.
		 */
		TestCaseTimeOutListener mTimeOutListener;
	};

	/**
	* @brief A test suite holds a list of test cases.
	*/
	class TestSuite : public MAUtil::TimerListener
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestSuite(const MAUtil::String& name);

		/**
		 * @brief Destructor.
		 */
		virtual ~TestSuite();

		/**
		 * @brief Add a test case to the suite.
		 */
		virtual void addTestCase(TestCase* testCase);

		/**
		 * @brief Run the next test case.
		 */
		virtual void runNextCase();
		virtual void runNextCaseHelper();
		virtual void runTimerEvent();

		/**
		 * @brief Get the name of the suite.
		 */
		virtual const MAUtil::String& getName() const;

		/**
		 * @brief Add listener that will be notified of test results.
		 */
		virtual void addTestListener(TestListener* testListener);

		/**
		 * @brief Send test suite begin event to listeners.
		 */
		virtual void fireBeginTestSuite(const MAUtil::String& suiteName);

		/**
		 * @brief Send test suite end event to listeners.
		 */
		virtual void fireEndTestSuite();

		/**
		 * @brief Send test case begin event to listeners.
		 */
		virtual void fireBeginTestCase(const MAUtil::String& testCaseName);

		/**
		 * @brief Send test case end event to listeners.
		 */
		virtual void fireEndTestCase();

		/**
		 * @brief Send assertion event to listeners.
		 */
		virtual void fireAssertion(const MAUtil::String& assertionName, bool success);

		/**
		 * @brief Send expectation event to listeners.
		 */
		virtual void fireExpectation(const MAUtil::String& assertionName);

		/**
		 * @brief Send timed out event to listeners.
		 */
		virtual void fireTimedOut(const MAUtil::String& testCaseName);

	protected:
		/**
		 * @brief Name of test suite.
		 */
		MAUtil::String mName;

		/**
		 * @brief List of test cases.
		 */
		MAUtil::Vector<TestCase*> mTestCases;

		/**
		 * List of test listeners.
		 */
		MAUtil::Vector<TestListener*> mTestListeners;

		/**
		 * @brief Index that point sto the current test case.
		 */
		int mCurrentTestCase;

		/**
		 * @brief Counter that tracks calls to runNextCase.
		 */
		int mRunCounter;
	};

} // namespace

#endif

// Include this for backwards compatibility for
// code that includes Test.h and and uses 
// XMLOutputTestListener (previously this class
// was declared in Test.h).
#include "XMLOutputTestListener.h"
