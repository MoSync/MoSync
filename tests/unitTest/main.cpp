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

		// automated tests
#ifdef USE_AUTOMATED_TESTS
		addMAUtilTypeTests(&mSuite);
		addConnTests(&mSuite);
		addDownloaderTests(&mSuite);	//right softkey press may be used during test; release is then passed to next test, failing it.
#endif
#ifdef USE_INTERACTIVE_TESTS
		// interactive tests
		addResTests(&mSuite);
#ifndef MA_PROF_SUPPORT_STYLUS		
		addCharInputTests(&mSuite);
#endif		
		addFramebufferTests(&mSuite);
		addSoundTests(&mSuite);
		addAdvGfxTests(&mSuite);
		addBasicGfxTests(&mSuite);
#ifndef MA_PROF_SUPPORT_STYLUS				
		addKeypadTests(&mSuite);
#endif		
		addTimeTests(&mSuite);
		addStoreTests(&mSuite);
		addSystemTests(&mSuite);
		addMemTests(&mSuite);
		addMathTests(&mSuite);
#endif

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
		if(complete && keyCode == MAK_0)
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
