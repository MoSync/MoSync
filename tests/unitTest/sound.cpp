/*
Copyright (C) 2011 MoSync AB

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

