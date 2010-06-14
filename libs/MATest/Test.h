/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** 
* \file Test.h
* \brief Testing framework for testing libraries and syscalls
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_TEST_H_
#define _SE_MSAB_TEST_H_

#include <ma.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

/**
* \brief MoSync test case classes
*/

namespace MATest {

	using namespace MAUtil;

	/**
	* \brief Listener for events triggered by the tests
	*/
	class TestListener {
	public:
		virtual void beginTestSuite(const String& suiteName);
		virtual void endTestSuite();
		virtual void beginTestCase(const String& testCaseName);
		virtual void endTestCase();
		virtual void assertion(const String& assertionName, bool cond);
		virtual ~TestListener();
	};

	/**
	* \brief Listener for events triggered by the tests and ouputs them to a store, as xml.
	*/
	class XMLOutputTestListener : public TestListener {
	private:
		String xmlOutput;
		String storeName;
		MAHandle placeHolder;
	public:
		XMLOutputTestListener(MAHandle placeholder, const String& storeName);
		~XMLOutputTestListener();
		void beginXML();
		void endXML();
		void beginTestSuite(const String &str);
		void endTestSuite();
		void beginTestCase(const String &str);
		void endTestCase();
		void assertion(const String &str, bool success);
	};


	class TestSuite;

	/**
	* \brief A test case.
	* 
	* Implement this class for each of your test cases. Use assert when testing
	* assertions. Override open and close to initialize and deinitialize test case 
	* specific things. Override start for the tests.
	*
	* Do not initialize any non-const variables in the constructor,
	* as the test may be run more than once.
	*/
	class TestCase {
	public:
		TestCase(const String& name);
		virtual ~TestCase();

		virtual void open();
		virtual void close();
		virtual void start() = 0;

		void addTestListener(TestListener *testListener);
		bool assert(const String& assertionName, bool success);
		const String& getName() const;
		void setSuite(TestSuite *suite);

	protected:
		TestSuite *suite;
		String name;
	};

	/**
	* \brief A list of TestCases. 
	* 
	* Add test cases with addTestCase.
	* Use runTestCases to run all test cases.
	*/
	class TestSuite {
	public:
		TestSuite(const String& name);
		~TestSuite();
		void addTestCase(TestCase *testCase);
		void runNextCase();
		const String& getName() const;
		void addTestListener(TestListener *testListener);
		void fireAssertion(const String& str, bool success);
	private:
		String name;
		Vector<TestCase*> testCases;
		Vector<TestListener*> testListeners;
		int mNextCase;
	};

}

#endif	//_SE_MSAB_TEST_H_
