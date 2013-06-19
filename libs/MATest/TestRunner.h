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
* @file TestRunner.h
* @brief High-level wrapper for defining and running test suites.
*/

#ifndef __MOSYNC_LIBS_MATEST_TESTRUNNER_H__
#define __MOSYNC_LIBS_MATEST_TESTRUNNER_H__

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Set.h>
#include <MAUtil/Environment.h>
#include "Test.h"

namespace MATest
{
	// ********** Class TestRunner **********

	/**
	 * @brief Class that registers and runs test cases.
	 *
	 * This class has one test suite that holds
	 * test cases. If you wish to use multiple test
	 * suites, you need to use the TestSuite class
	 * directly, and not use this class.
	 *
	 * This class is designed to be a singleton.
	 */
	class TestRunner
	{
	public:
		/**
		 * @brief Constructor.
		 */
		TestRunner();

		/**
		 * @brief Destructor.
		 */
		virtual ~TestRunner();

		/**
		 * @brief Add a TestCase to the test suite of
		 * this test runner.
		 */
		void addTestCase(MATest::TestCase* testCase);

		/**
		 * @brief Add a TestListener to the test suite of
		 * this test runner.
		 */
		void addTestListener(MATest::TestListener* listener);

		/**
		 * @brief Start running tests.
		 */
		void runTests();

		/**
		 * @brief Returns the single instance of this class.
		 * Creates the instance if needed.
		 */
		static TestRunner* getInstance();

	public:
		/**
		 * @brief Test suite that holds test cases.
		 */
		MATest::TestSuite mSuite;

		/**
		 * @brief Class variable to hold the global instance
		 * of this class.
		 */
		static TestRunner* sInstance;
	};

	// ********** Class RegisterTestCase **********

	/**
	 * @brief Helper class that is used to register test cases.
	 *
	 * Used with the macro TESTCASE. Instantiating a class at
	 * the "top level" of a source file is a "trick" to run
	 * code while a source file loads, as in scripting languages.
	 */
	class RegisterTestCase
	{
	public:
		/**
		 * Adds the test case to the singleton instance of
		 * class TestRunner.
		 */
		RegisterTestCase(TestCase* testCase);
	};

	// ********** Class HighLevelTestListener **********

	/**
	 * @brief A high level test listener.
	 *
	 * Implementation of a TestListener that can be used as is,
	 * or subclassed to provide futher capability, like output to
	 * a NativeUI widget or sending test results over the network.
	 *
	 * The method to subclass is endTestSuite.
	 */
	class HighLevelTestListener : public TestListener
	{
	public:

		// ***** Constructor/destructor *****

		/**
		 * @brief Constructor.
		 */
		HighLevelTestListener();

		/**
		 * @brief Destructor.
		 */
		virtual ~HighLevelTestListener();

		/**
		 * @brief Clear all test results.
		 */
		virtual void reset();

		// ***** Methods for accessing the test result *****

		/**
		 * @brief Get the number of test cases.
		 */
		virtual int getNumberOfTestCasesRan();

		/**
		 * @brief Get the number of test cases.
		 */
		virtual int getNumberOfTestCasesTimedOut();

		/**
		 * @brief Get the name of a timed out test case.
		 */
		virtual MAUtil::String getTestCaseTimedOut(int index);

		/**
		 * @brief Get the number of passed asserts.
		 */
		virtual int getNumberOfAssertsPassed();

		/**
		 * @brief Get the number of failed asserts.
		 */
		virtual int getNumberOfAssertsFailed();

		/**
		 * @brief Get the name of a failed assert.
		 */
		virtual MAUtil::String getAssertFailed(int index);

		/**
		 * @brief Get the number of expected asserts.
		 */
		virtual int getNumberOfAssertsExpected();

		/**
		 * @brief Get the name of an expected assert.
		 */
		virtual MAUtil::String getAssertExpected(int index);

		// ***** Methods inherited from TestListener *****

		/**
		 * @brief Does nothing.
		 */
		virtual void beginTestSuite(const MAUtil::String& suiteName);

		/**
		 * @brief Reports test results to the device screen using printf.
		 * Override to do your custom test reporting.
		 */
		virtual void endTestSuite();

		/**
		 * @brief Saves the name of the current test case.
		 * If you override this method, make sure you save
		 * the test case name, call the superclass method
		 * (the method in this class) to do so.
		 */
		virtual void beginTestCase(const MAUtil::String& testCaseName);

		/**
		 * @brief Increments the counter for number of test cases ran.
		 */
		virtual void endTestCase();

		/**
		 * @brief Adds failed asserts to the  a list. Increments passed asserts.
		 * Removes any passed asserts present in the list of expected asserts.
		 */
		virtual void assertion(const MAUtil::String& assertionName, bool cond);

		/**
		 * @brief Adds the expect to mExpectedAsserts.
		 */
		virtual void expectation(const MAUtil::String& assertionName);

		/**
		 * @brief Adds the timed out test case to mTimedOutTestCases.
		 */
		virtual void timedOut(const MAUtil::String& testCaseName);

	protected:

		// ***** Instance variables *****

		/**
		 * @brief Number of test cases ran.
		 */
		int mNumTestCasesRan;

		/**
		 * @brief Names of timed out test cases.
		 */
		MAUtil::Vector<MAUtil::String> mTestCasesTimedOut;

		/**
		 * @brief Number of asserts passed.
		 */
		int mNumAssertsPassed;

		/**
		 * @brief Names of failed asserts.
		 */
		MAUtil::Vector<MAUtil::String> mAssertsFailed;

		/**
		 * @brief Holds expected asserts.
		 * At the end of all tests, elements in this list
		 * are asserts that never happened. If all expects
		 * happend, the size of this set is zero.
		 */
		MAUtil::Vector<MAUtil::String> mAssertsExpected;

		/**
		 * @brief The name of the currently running test case.
		 * Used internally.
		 */
		MAUtil::String mCurrentTestCaseName;
	};

	// ********** Macros **********

	// Macro to make writing tests easier.
	#define TESTCASE(testCaseClass) \
		static MATest::TestCase* sTestCase = \
			new testCaseClass(#testCaseClass); \
		static MATest::RegisterTestCase __testCaseClass(sTestCase);

} // namespace

#endif
