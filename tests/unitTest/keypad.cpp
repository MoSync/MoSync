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
#include "common.h"

//pressing and releasing the correct key succeeds the test
//klicking any other key twice in a row fails the test

class KeyCase : public KeyBaseCase {
public:
	KeyCase(int code, const String& name) : KeyBaseCase(name), mCode(code) {}

	//TestCase
	void start() {
		mPressed = mReleased = 0;
		printf("Press %s\n", name.c_str());
	}

	//KeyListener
	void keyPressEvent(int keyCode) {
		mPressed = keyCode;
	}
	void keyReleaseEvent(int keyCode) {
		if(keyCode == mPressed) {
			if(keyCode == mCode) {
				assert(name, true);
				suite->runNextCase();
			} else if(keyCode == mReleased) {
				assert(name, false);
				suite->runNextCase();
			}
			mReleased = keyCode;
		}
		mPressed = 0;
	}
private:
	const int mCode;
	int mPressed, mReleased;
};

void addKeypadTests(TestSuite* suite);
void addKeypadTests(TestSuite* suite) {
	suite->addTestCase(new KeyCase(MAK_FIRE, "fire"));
	suite->addTestCase(new KeyCase(MAK_UP, "up"));
	suite->addTestCase(new KeyCase(MAK_DOWN, "down"));
	suite->addTestCase(new KeyCase(MAK_LEFT, "left"));
	suite->addTestCase(new KeyCase(MAK_RIGHT, "right"));
	suite->addTestCase(new KeyCase(MAK_SOFTLEFT, "softkey left"));
	suite->addTestCase(new KeyCase(MAK_SOFTRIGHT, "softkey right"));
	suite->addTestCase(new KeyCase(MAK_STAR, "*"));
	suite->addTestCase(new KeyCase(MAK_POUND, "#"));
	suite->addTestCase(new KeyCase(MAK_CLEAR, "clear"));

	//suite->addTestCase(new KeyCase(MAK_0, "0"));
#define ADD_NUMBER_CASE(n) suite->addTestCase(new KeyCase(MAK_##n, #n));
#define NUMBER_KEYS(m) m(0) m(1) m(2) m(3) m(4)	m(5) m(6) m(7) m(8) m(9)

	NUMBER_KEYS(ADD_NUMBER_CASE);
}
