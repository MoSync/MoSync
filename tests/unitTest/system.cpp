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

#include <mavsprintf.h>
#include "common.h"
#include <conprint.h>

#ifdef MAPIP
#include <maprofile.h>
#endif

class ResetBacklightCase : public KeyBaseCase, public TimerListener {
public:
	ResetBacklightCase(const String& name) : KeyBaseCase(name) {}

	//TestCase
	virtual void start() {
		clearScreen();
		Environment::getEnvironment().addTimer(this, 100, -1);
	}

	virtual void close() {
		KeyBaseCase::close();
		Environment::getEnvironment().removeTimer(this);
	}

	void runTimerEvent() {
		clearScreen();
		maSetColor(GREEN);
		maDrawText(2, 2, "Is backlight reset all the time?");
		maResetBacklight();
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
	}
	virtual void keyReleaseEvent(int keyCode) {
		checkYesNo(keyCode);
	}
};


class KeyLockCase : public KeyBaseCase, public TimerListener {
public:
	KeyLockCase(const String& name) : KeyBaseCase(name) {}

	//TestCase
	virtual void start() {

		clearScreen();
		Environment::getEnvironment().addTimer(this, 6000, 2);
		printf("press keys\n");
		mode = 0;
		keyPressFlag = 0;
	}

	virtual void close() {
		KeyBaseCase::close();
		Environment::getEnvironment().removeTimer(this);
	}

	void runTimerEvent() {
		mode++;
		if(mode == 1) {
			if(!assert("maLockKeypad available?", maLockKeypad()>=0)) {
				suite->runNextCase();
				return;
			}
			printf("key lock on\n");
			printf("try to press keys\n");
			assert("Is locked?", maKeypadIsLocked()==1);
		} else if( mode == 2) {
			printf("key lock off\n");
			printf("keys should be pressable again?\n");
			maUnlockKeypad();
			assert("Isn't locked?", maKeypadIsLocked()==0);
		}
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
		printf("key %d pressed\n", keyCode);
	}

	virtual void keyReleaseEvent(int keyCode) {
		if(mode == 2)
			checkYesNo(keyCode);
	}

private:
	int mode;
	int keyPressFlag;
};


#ifdef MA_PROF_SUPPORT_STYLUS
#define VIB_MSG "press on screen to turn of vibration\n"
#else
#define VIB_MSG "press fire to turn of vibration\n"
#endif

class VibrateCase : public KeyBaseCase {
public:
	VibrateCase(const String& name) : KeyBaseCase(name) {}

	//TestCase
	virtual void start() {
		if(!assert("maVibrate available?",maVibrate(100000)!=0)) {
			suite->runNextCase();
			return;
		}
		clearScreen();
		printf(VIB_MSG);
		on = true;
	}

	virtual void close() {
		KeyBaseCase::close();
	}

	//KeyListener
	virtual void keyPressEvent(int keyCode) {
		if(keyCode == MAK_FIRE) {
			maVibrate(0);
			on = false;
			printf("did you turn off the vibration?\n");
		}
	}

	void pointerPressEvent(MAPoint2d p) {
		if(on) {
			maVibrate(0);
			on = false;
			printf("did you turn off the vibration?\n");
		} else {
			KeyBaseCase::pointerReleaseEvent(p);
		}
	}

	void pointerReleaseEvent(MAPoint2d p) {
	}


	virtual void keyReleaseEvent(int keyCode) {
		if(!on)
			checkYesNo(keyCode);
	}

private:
	bool on;
};

class BatteryChargeCase : public TestCase {
public:
	BatteryChargeCase(const String& name) : TestCase(name) {}

	virtual void start() {
		int charge;
		if(assert("maGetBatteryCharge available?",(charge=maGetBatteryCharge())>=0))
			assert("maGetBatteryCharge in range", charge>=0&&charge<=100); // if the battery charge is zero, then the phone wouldn't be on right?
		suite->runNextCase();
	}

};

void addSystemTests(TestSuite* suite);
void addSystemTests(TestSuite* suite) {
	suite->addTestCase(new ResetBacklightCase("maResetBacklightTest"));
#ifndef MA_PROF_SUPPORT_STYLUS
	suite->addTestCase(new KeyLockCase("maLockKeypadTest"));
	suite->addTestCase(new VibrateCase("maVibrateTest"));
	suite->addTestCase(new BatteryChargeCase("maGetBatteryCharge"));
#endif

}
