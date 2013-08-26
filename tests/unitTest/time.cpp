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

#include <conprint.h>
#include <matime.h>
#include "common.h"

int localTime();
int localTime() {
	printf("Local time:\n");
	printf("%s\n", sprint_time(maLocalTime()));
	return 0;
}

int utcTime();
int utcTime() {
	printf("UTC time:\n");
	printf("%s\n", sprint_time(maTime()));
	return 0;
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

void addTimeTests(TestSuite* suite);
void addTimeTests(TestSuite* suite) {
	suite->addTestCase(new MaWaitCase);
	suite->addTestCase(new MobletTimerCase);
	suite->addTestCase(new TemplateCase<localTime>("localTime"));
	suite->addTestCase(new TemplateCase<utcTime>("utcTime"));
}
