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

/** \file Test.cpp
*/

#include <conprint.h>
#include <matime.h>
#include "MAUtil/String.h"
#include "Test.h"

#define XML_LOGGING
#ifdef XML_LOGGING
#define XML_LOG lprintfln
#else
#define XML_LOG(...)
#endif

namespace MATest {

	using namespace MAUtil;

	void TestListener::beginTestSuite(const String& suiteName) {
	}

	void TestListener::endTestSuite(){
	}

	void TestListener::beginTestCase(const String& testCaseName){
	}

	void TestListener::endTestCase(){
	}

	void TestListener::assertion(const String& assertionName, bool cond){
	}

	TestListener::~TestListener() {
	}


	XMLOutputTestListener::XMLOutputTestListener(MAHandle placeholder, const String& storeName) {
		this->storeName = storeName;
		this->placeHolder = placeholder;

		beginXML();
	}

	XMLOutputTestListener::~XMLOutputTestListener() {
		endXML();
	}
	void XMLOutputTestListener::beginXML() {
		xmlOutput="";
		xmlOutput+="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
		xmlOutput+="<TestReport time=\"";
		xmlOutput+=sprint_time(maLocalTime());
		xmlOutput+="\">\n";
	}
	void XMLOutputTestListener::endXML() {
		XML_LOG("endXML");
		xmlOutput+="</TestReport>\n";
#ifdef XML_LOGGING
		maWriteLog(xmlOutput.c_str(), xmlOutput.size());
#endif
		MAHandle store = maOpenStore(storeName.c_str(), MAS_CREATE_IF_NECESSARY);
		maCreateData(placeHolder, xmlOutput.size());
		maWriteData(placeHolder, xmlOutput.c_str(), 0, xmlOutput.size());
		maWriteStore(store, placeHolder);
		maCloseStore(store, 0);
	}
	void XMLOutputTestListener::beginTestSuite(const String &str) {
		XML_LOG("---------------------------------------");
		XML_LOG("test suite: %s", str.c_str());
		XML_LOG("---------------------------------------");

		beginXML();

		xmlOutput+="\t<TestSuite name=\""+str+"\">\n";
	}

	void XMLOutputTestListener::endTestSuite() {
		xmlOutput+="\t</TestSuite>\n";

		endXML();
	}

	void XMLOutputTestListener::beginTestCase(const String &str) {
		XML_LOG("---------------------------------------");
		XML_LOG("test case: %s", str.c_str());
		XML_LOG("---------------------------------------");

		xmlOutput+="\t\t<TestCase name=\""+str+"\">\n";
	}

	void XMLOutputTestListener::endTestCase() {
		xmlOutput+="\t\t</TestCase>\n";
	}

	void XMLOutputTestListener::assertion(const String &str, bool cond) {
		String res;
		if(cond) {
			XML_LOG("%s, succeeded.", str.c_str());
			res = "true";
		}
		else {
			XML_LOG("%s, failed.", str.c_str());
			res = "false";
		}
		xmlOutput+="\t\t\t<Assertion name=\"" + str + "\" result=\"" + res + "\"/>\n";
	}

	TestCase::TestCase(const String& name) : name(name) {
	}

	TestCase::~TestCase() {

	}

	void TestCase::open() {

	}

	void TestCase::close() {
	}

	bool TestCase::assert(const String& assertionName, bool cond) {
		suite->fireAssertion(assertionName, cond);
		return cond;
	}

	const String& TestCase::getName() const {
		return name;
	}

	void TestCase::setSuite(TestSuite *suite) {
		this->suite = suite;
	}


	TestSuite::TestSuite(const String& name) : name(name), mNextCase(0) {
	}

	TestSuite::~TestSuite() {
	}

	void TestSuite::addTestCase(TestCase *testCase) {
		testCases.add(testCase);
		testCase->setSuite(this);
	}

#if 0
	void TestSuite::runTestCases() {
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->beginTestSuite(name);
		}
		for(int i = 0; i < testCases.size(); i++) {
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->beginTestCase(testCases[i]->getName());
			}
			testCases[i]->open();
			testCases[i]->run();
			testCases[i]->close();
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->endTestCase();
			}
		}
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->endTestSuite();
		}
	}
#endif
	void TestSuite::runNextCase() {
		if(mNextCase == 0) {
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->beginTestSuite(name);
			}
		} else {
			testCases[mNextCase - 1]->close();
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->endTestCase();
			}
		}
		if(mNextCase == testCases.size()) {
			for(int j = 0; j < testListeners.size(); j++) {
				testListeners[j]->endTestSuite();
			}
			return;
		}

		int curCase = mNextCase++;
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->beginTestCase(testCases[curCase]->getName());
		}
		testCases[curCase]->open();
		testCases[curCase]->start();
	}

	const String& TestSuite::getName() const {
		return name;
	}

	void TestSuite::addTestListener(TestListener *testListener) {
		testListeners.add(testListener);
	}

	void TestSuite::fireAssertion(const String& str, bool cond) {
		for(int j = 0; j < testListeners.size(); j++) {
			testListeners[j]->assertion(str, cond);
		}
	}

}
