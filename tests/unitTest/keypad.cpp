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
