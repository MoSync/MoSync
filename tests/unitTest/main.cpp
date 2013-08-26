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

#include <MAUtil/Moblet.h>
#include <MATest/Test.h>
#include <conprint.h>

#ifdef MAPIP
#include <maprofile.h>
#endif

#include "common.h"

using namespace MAUtil;

#define USE_AUTOMATED_TESTS
#define USE_INTERACTIVE_TESTS

// automated tests
void addDownloaderTests(TestSuite* suite);
void addMAUtilTypeTests(TestSuite* suite);
void addConnTests(TestSuite* suite);
void addMAUtilUtilTests(TestSuite* suite);

// interactive tests
//doesn't check pixel-perfection, except in a few cases.
void addBasicGfxTests(TestSuite* suite);
void addAdvGfxTests(TestSuite *suite);
void addKeypadTests(TestSuite* suite);
void addTimeTests(TestSuite* suite);
void addStoreTests(TestSuite* suite);
void addSoundTests(TestSuite* suite);
void addFramebufferTests(TestSuite* suite);
void addSystemTests(TestSuite* suite);
void addMemTests(TestSuite* suite);
void addMathTests(TestSuite* suite);
void addCharInputTests(TestSuite* suite);
void addResTests(TestSuite* suite);

static int lastPress = -2000;

class MyMoblet : public Moblet, public TestListener {
private:
	struct Failure {
		String caseName, assertName;
	};

	TestSuite mSuite;
	bool complete;
	String mCurrentTestName;
	Vector<Failure> mFailures;
	int mNSuccesses;
public:
	MyMoblet() : mSuite("MoSync") {

#if 1
		// automated tests
#ifdef USE_AUTOMATED_TESTS
		addResTests(&mSuite);
		addMAUtilTypeTests(&mSuite);
		addMAUtilUtilTests(&mSuite);
		addMemTests(&mSuite);
		addMathTests(&mSuite);
		addStoreTests(&mSuite);
		addConnTests(&mSuite);
		addDownloaderTests(&mSuite);	//right softkey press may be used during test; release is then passed to next test, failing it.
#endif	//USE_AUTOMATED_TESTS

#ifdef USE_INTERACTIVE_TESTS
		// interactive tests
#ifndef MA_PROF_SUPPORT_STYLUS
		addCharInputTests(&mSuite);
#endif	//MA_PROF_SUPPORT_STYLUS
		addFramebufferTests(&mSuite);
		addSoundTests(&mSuite);
		addAdvGfxTests(&mSuite);
		addBasicGfxTests(&mSuite);
#ifndef MA_PROF_SUPPORT_STYLUS
		addKeypadTests(&mSuite);
#endif	//MA_PROF_SUPPORT_STYLUS
		addTimeTests(&mSuite);
		addSystemTests(&mSuite);
#endif	//USE_INTERACTIVE_TESTS
#endif	//1

		mSuite.addTestListener(new XMLOutputTestListener(maCreatePlaceholder(), "unitTest.xml"));
		mSuite.addTestListener(this);
		complete = false;

		mSuite.runNextCase();
	}

	void beginTestSuite(const String& suiteName) {
		//printf("Test Suite begin: %s\n", suiteName.c_str());	//mustn't trash the screen here
		printf("Test begins..\nPlease wait,\nrunning automated tests!\n");
		mNSuccesses = 0;
		mFailures.clear();
		mCurrentTestName.clear();
	}

	void endTestSuite() {
		printf("Test Suite complete.\n");
		printf("Press 0 to exit.\n");
		printf("%i successes.\n", mNSuccesses);
		printf("%i failures.\n", mFailures.size());
		for(int i=0; i<mFailures.size(); i++) {
			printf("%s @ %s\n", mFailures[i].assertName.c_str(), mFailures[i].caseName.c_str());
		}
		complete = true;
	}

	void beginTestCase(const String& testCaseName) {
		mCurrentTestName = testCaseName;
	}
	void endTestCase() {
		mCurrentTestName.clear();
	}
	void assertion(const String& assertionName, bool cond) {
		if(!cond) {
			Failure f;
			f.caseName = mCurrentTestName;
			f.assertName = assertionName;
			mFailures.add(f);
		} else {
			mNSuccesses++;
		}
	}

	void keyPressEvent(int keyCode) {
		if(complete && keyCode == MAK_0 || keyCode == MAK_BACK)
			close();
	}

	void pointerPressEvent(MAPoint2d p) {

		int currentPress = maGetMilliSecondCount();

		if((currentPress - lastPress) < 300) {
			if(complete) close();
		} else {
			lastPress = currentPress;
		}
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	Moblet::run(new MyMoblet());
	return 0;
};
