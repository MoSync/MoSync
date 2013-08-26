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
		suite->runNextCase();
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
		{
			checkYesNo(keyCode);
			suite->runNextCase();
		}
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
		{
			checkYesNo(keyCode);
			suite->runNextCase();
		}
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
