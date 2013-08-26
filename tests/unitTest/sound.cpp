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
#include "MAHeaders.h"

class SoundCase : public KeyBaseCase {
public:
	SoundCase() : KeyBaseCase("maSoundPlay") {}

	//TestCase
	void start() {
		assert("maSoundPlay", maSoundPlay(RES_MP3, 0, maGetDataSize(RES_MP3))>=0);
		suite->runNextCase();
	}

	void keyPressEvent(int keyCode) {
	}

	void keyReleaseEvent(int keyCode) {
		checkYesNo(keyCode);
		suite->runNextCase();
	}
};

void addSoundTests(TestSuite* suite);
void addSoundTests(TestSuite* suite) {
	suite->addTestCase(new SoundCase());
}

