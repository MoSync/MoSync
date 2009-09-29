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

#include <conprint.h>
#include <matime.h>
#include "common.h"

void localTime() {
	printf("Local time:\n");
	printf("%s\n", sprint_time(maLocalTime()));
}

void utcTime() {
	printf("UTC time:\n");
	printf("%s\n", sprint_time(maTime()));
}

#define TEST_TIMEOUT 1000
#define TEST_TIME_EPSILON 30	//we allow for 30 ms error

class MaWaitCase : public TestCase {
public:
	MaWaitCase() : TestCase("maWait") {}

	//TestCase
	void start() {
		printf("Don't touch anything now...\n");
		MAEvent e;
		while(maGetEvent(&e)) {	//clear event buffer
			if(e.type == EVENT_TYPE_CLOSE)
				maExit(0);
		}
		int startTime = maGetMilliSecondCount();
		maWait(TEST_TIMEOUT);
		int delta = maGetMilliSecondCount() - startTime;
		bool success = delta > TEST_TIMEOUT - TEST_TIME_EPSILON;
		printf("%s (%i ms)\n", success ? "Success" : "Failure", delta);
		assert(name, success);
		suite->runNextCase();
	}
};

class MobletTimerCase : public TestCase, public TimerListener {
public:
	MobletTimerCase() : TestCase("mobletTimer") {}

	//TestCase
	void start() {
		printf("You can touch stuff now if you like...\n");
		mStartTime = maGetMilliSecondCount();
		Environment::getEnvironment().addTimer(this, TEST_TIMEOUT, 1);
	}

	//TimerListener
	void runTimerEvent() {
		int delta = maGetMilliSecondCount() - mStartTime;
		bool success = delta > TEST_TIMEOUT - TEST_TIME_EPSILON;
		printf("%s (%i ms)\n", success ? "Success" : "Failure", delta);
		assert(name, success);
		suite->runNextCase();
	}

private:
	int mStartTime;
};

void addTimeTests(TestSuite* suite) {
	suite->addTestCase(new MaWaitCase);
	suite->addTestCase(new MobletTimerCase);
	suite->addTestCase(new TemplateCase<localTime>("localTime"));
	suite->addTestCase(new TemplateCase<utcTime>("utcTime"));
}
